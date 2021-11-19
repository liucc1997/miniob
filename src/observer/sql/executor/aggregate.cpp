#include "sql/executor/aggregate.h"
// #include "common/log/log.h"
#include "storage/common/date.h"


Aggregate* Aggregate::aggregate_allocator(Table *table, AggregationType type, const char* attr_name) 
{
    const FieldMeta *field_meta = table->table_meta().field(attr_name);
    if (type == AGCOUNT) {
        if (field_meta == nullptr && 
            strcmp(attr_name, "*") != 0 && strcmp(attr_name, "") != 0   //count(*) or count()
        ) {
            return nullptr;
        }
        return new CountAggregate(type, attr_name);
    }
    // 对于AVG MIN MAX 都需要指定attribute name
    if (field_meta == nullptr) {
        return nullptr;
    } 
    size_t feild_offset = field_meta->offset();
    AttrType feild_type = field_meta->type();
    if (type == AGMAX)  {
        return new MaxAggregate(type, attr_name, feild_offset, feild_type);
    }
    if (type == AGMIN)  {
        return new MinAggregate(type, attr_name, feild_offset, feild_type);
    }
    if (type == AGAVG && (feild_type == INTS || feild_type == FLOATS) ) {
        return new AvgAggregate(type, attr_name, feild_offset, feild_type);
    }
    return nullptr;
}

void MaxAggregate::do_record(const char *record) {
    if (!value_data_) {
        value_data_ = record + feild_offset_;
    }
    int compare_ret = 0;
    if (feild_type_ == INTS || feild_type_ == DATES) {
        int v = *((int*)value_data_);
        int record_v = *((int*)(record + feild_offset_));
        if (record_v > v) {
            value_data_ = record + feild_offset_;
        }
    }
    if (feild_type_ == FLOATS ) {
        float v = *((float*)value_data_);
        float record_v = *((float*)(record + feild_offset_));
        if (record_v > v) {
            value_data_ = record + feild_offset_;
        }
    }
    if (feild_type_ == CHARS ) {
        const char* v = value_data_;
        const char* record_v = record + feild_offset_;
        if (strcmp(record_v, v) > 0) {
            value_data_ = record + feild_offset_;
        }
    }
}

void MinAggregate::do_record(const char* record) {
    if (!value_data_) {
        value_data_ = record + feild_offset_;
    }
    int compare_ret = 0;
    if (feild_type_ == INTS || feild_type_ == DATES) {
        int v = *((int*)value_data_);
        int record_v = *((int*)(record + feild_offset_));
        if (record_v < v) {
            value_data_ = record + feild_offset_;
        }
    }
    if (feild_type_ == FLOATS ) {
        float v = *((float*)value_data_);
        float record_v = *((float*)(record + feild_offset_));
        if (record_v < v) {
            value_data_ = record + feild_offset_;
        }
    }
    if (feild_type_ == CHARS ) {
        const char* v = value_data_;
        const char* record_v = record + feild_offset_;
        if (strcmp(record_v, v) < 0) {
            value_data_ = record + feild_offset_;
        }
    }
}

void AvgAggregate::do_record(const char *record) {
    if (feild_type_ == INTS) {
        int v = *((int*)(record + feild_offset_));
        avg_ = avg_ * (count_*1.0/(count_+1)) + v * 1.0 / (count_ + 1);
        count_ ++;
    }
    if (feild_type_ == FLOATS) {
        float v = *((float*)(record + feild_offset_));
        avg_ = avg_ * (count_*1.0/(count_+1)) + v / (count_ + 1);
        count_ ++;
    }
}

void CompositeAggregate::do_record(const char *record) {
    for (std::vector<Aggregate*>::iterator iter = aggres_.begin(); iter != aggres_.end(); iter++) {
        (*iter)->do_record(record);
    }
}
void CompositeAggregate::set_tuple(TupleSet &tuple_set) {
    Tuple tuple;
    for (Aggregate* a : aggres_) {
        if (a->get_result_poniter() == nullptr) {
            printf("Failed to get result from Aggregate. %s", a->get_schema_attr());
            return;
        }
        const char *p = a->get_result_poniter();
        if (a->get_aggr_type() == AGCOUNT) {
            tuple.add(*(int*)p);
        }
        if (a->get_aggr_type() == AGAVG) {
            double t = *(double*)p;
            tuple.add((float)t);
        }        
        if (a->get_aggr_type() == AGMAX || a->get_aggr_type() == AGMIN) {
            if (a->get_attr_type() == INTS) {
                tuple.add(*(int*)p);
            }
            if (a->get_attr_type() == FLOATS) {
                tuple.add(*(float*)p);
            }
           if (a->get_attr_type() == CHARS) {
                tuple.add(p, strlen(p));
            }     
            if (a->get_attr_type() == DATES) {
                int t = *(int*)p;
                Date d(t);
                tuple.add(d.to_string(), DATA_STRING_LEN);
            }
        }
    }
    tuple_set.add(std::move(tuple));
}

CompositeAggregate::~CompositeAggregate () {
    for (int i = 0; i < aggres_.size(); i++) {
        printf("<LCC> ~CompositeAggregate: delete a%d.\n", i);
        delete aggres_[i];
    }
    aggres_.clear();
}
