#include <vector>
#include "storage/common/table_meta.h"
#include "storage/common/table.h"
#include "sql/executor/tuple.h"

class Aggregate {
    public:
    static void record_reader(const char *data, void *context) {
        Aggregate *converter = (Aggregate *)context;
        converter->do_record(data);
    }
    static RC record_reader_ret(Record *record, void *context) {
        Aggregate *converter = (Aggregate *)context;
        converter->do_record((char*)record);
        return RC::SUCCESS;
    }
    Aggregate(AggregationType type, const char* attr_name) : aggr_type_(type), attr_name_(attr_name), feild_type_(UNDEFINED) {}
    virtual void do_record(const char *record) = 0;
    virtual const char* get_result_poniter() {
        return nullptr;
    }    
    static Aggregate* aggregate_allocator(Table *table, AggregationType type, const char* attr_name);
    virtual const char* get_schema_attr() { return schema_attr_;}
    AttrType get_attr_type() { return feild_type_;}
    AggregationType get_aggr_type() { return aggr_type_; }
    protected:
    char schema_attr_[16];
    size_t feild_offset_;
    AttrType feild_type_;
    AggregationType aggr_type_;
    const char* attr_name_;
};

class CountAggregate : public Aggregate {
    private:
    int count_;
    public:
    CountAggregate(AggregationType type, const char* attr_name) : count_(0), Aggregate(type, attr_name) {}
    void do_record(const char *record) {
        count_ ++;
    }
    virtual const char* get_result_poniter() {
        return (char*)(&count_);
    }
    virtual const char* get_schema_attr() { 
        snprintf(schema_attr_, 16, "count(%s)", attr_name_);
        return schema_attr_;
    }
};

class MaxAggregate : public Aggregate {
    public:
    MaxAggregate(AggregationType type, const char* attr_name, size_t feild_offset, AttrType feild_type) : 
        Aggregate(type, attr_name), value_data_(nullptr)
    {
        feild_offset_ = feild_offset;
        feild_type_ = feild_type;
    }
    void do_record(const char *record);
    virtual const char* get_result_poniter() {
        return value_data_;
    }
    virtual const char* get_schema_attr() { 
        snprintf(schema_attr_, 16, "max(%s)", attr_name_);
        return schema_attr_;
    }
    protected:
    const char* value_data_;
};

class MinAggregate : public MaxAggregate {
    public:
    MinAggregate(AggregationType type, const char* attr_name, size_t feild_offset, AttrType feild_type) : 
        MaxAggregate(type, attr_name, feild_offset, feild_type) {}
    void do_record(const char *record);
    virtual const char* get_schema_attr() { 
        snprintf(schema_attr_, 16, "min(%s)", attr_name_);
        return schema_attr_;
    }
};

class AvgAggregate : public Aggregate {
    public:
    AvgAggregate(AggregationType type, const char* attr_name, size_t feild_offset, AttrType feild_type) : 
        Aggregate(type, attr_name), avg_(0), count_(0) 
    {
        feild_offset_ = feild_offset;
        feild_type_ = feild_type;
    }
    void do_record(const char *record);
    virtual const char* get_result_poniter() {
        return (char*)(&avg_);
    }
    virtual const char* get_schema_attr() { 
        snprintf(schema_attr_, 16, "avg(%s)", attr_name_);
        return schema_attr_;
    }
    private:
    double avg_;
    size_t count_;
};

class CompositeAggregate : public Aggregate {
    public:
    void add_aggration(Aggregate* a) {
        aggres_.push_back(a);
    }
    CompositeAggregate () : Aggregate(UNKNOW_AGTYPE, nullptr) {}
    ~CompositeAggregate ();
    std::vector<Aggregate*>& get_aggre_list() { return aggres_; }
    void do_record(const char *record);
    virtual char* get_schema_attr() { return nullptr; }
    void set_tuple(TupleSet &tuple_set);
    private:
    std::vector<Aggregate*> aggres_;
};