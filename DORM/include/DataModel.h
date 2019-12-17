#ifndef DATAMODEL_H
#define DATAMODEL_H
#include <memory>
#include "Singleton.h"
#include <unordered_map>
#include <list>
#include <mutex>
#include <Util.h>
#include <algorithm>

/***************classes**************/
template <class DataObjType>
struct Hasher;

template <class DataObjType>
class Key;

template <class DataContainerType>
class DataModelUpdater;

template <class DataObjType>
class DataModel;

/************************************/

using std::hash;
//The STL map type used in the DataModel
template <class DataObjType>
using DataModelMap =  std::unordered_map<Key<DataObjType>, DataObjType , Hasher<DataObjType> >;


/*****************************************************************
 * Class Name      : Key
 *
 * Base Class      : None
 *
 * Purpose         : Used as the key object to be stored in the underlying STL map
 *
 * Special Members : keyId - uniqueue key ID
 * hashValue - used to hash the unordered_map
 * templates       :
 * DataObjType - The object type assigned by the user to be stored in the underlying map
 *
 ****************************************************************/
template <class DataObjType>
struct Key
{
private:
    unsigned int keyId;   //unique key
    size_t hashValue;	  //not necessarily unique

public:

    Key (DataObjType dataObj)
    {
        setVals(dataObj);
    }
    Key () {}

    //Setter: keyId and hashValue, if the DataObjType is a pointer
    template <class TT = DataObjType>
    void setVals(TT dataObj, typename std::enable_if<std::is_pointer<TT>::value>::type* = nullptr)
    {
        keyId =  int(dereference(dataObj));
        hashValue = hash<std::string>()(std::string(dereference(dataObj)));

    }

    //Setter: keyid and hashValue, if the DataObjType is not a pointer
    template <class TT = DataObjType>
    void setVals(TT dataObj, typename std::enable_if<!std::is_pointer<TT>::value>::type* = nullptr)
    {
        keyId =  int(dereference(dataObj));
        hashValue = hash<std::string>()(std::string(dereference(dataObj)));

    }


    //Equality is only decided by the unique id
    bool operator==(const Key &otherKey) const
    {
        unsigned int otherId;
        otherId = otherKey.getKeyId();
        return (keyId == otherId)? true: false;
    }

    //Getter: keyId
    unsigned int getKeyId() const
    {
        return keyId;
    }

    //Getter: hashValue
    std::size_t getHashValue() const
    {
        return hashValue;
    }
private:
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & keyId;
        ar & hashValue;
    }


};

//Hasher to be used for the unorderd_map elements
template <class DataObjType>
struct Hasher : public std::unary_function<Key<DataObjType>, size_t>
{
    std::size_t operator()(const Key<DataObjType>& k) const
    {
        return k.getHashValue();
    }
};

//The class is used to create a functor for retrieving each element from the DataModelMap
template <class DataObjType>
struct getSecond : public std::unary_function<typename DataModelMap<DataObjType>::value_type, DataObjType>
{
    DataObjType operator()(const typename DataModelMap<DataObjType>::value_type& value) const
    {
        return value.second;
    }
};


/*****************************************************************
 * Class Name      : DataModel
 *
 * Base Class      : Singleton
 *
 * Purpose         : This class is the container of the dataObjectMap
 *
 * Special Members : dataObjectMap - unorderd_map that holds user objects
 * templates       : DataContainerType - User defined object type
 *
 ****************************************************************/
template <class DataContainerType>
class DataModel: public Singleton< DataModel<DataContainerType> >
{
    friend class Singleton<DataModel<DataContainerType> >;
private:
    DataModelMap<DataContainerType> dataObjectMap;
    DataModel() {}
    ~DataModel() {}

public:
    static void Destroy() {
        Singleton<DataModel<DataContainerType> >::Destroy();
    }

    //Returns the object of DataContainerType when a key is parsed
    const DataContainerType& getDataObjByKey(const Key<DataContainerType>& key)
    {
        typename DataModelMap<DataContainerType>::const_iterator itr = dataObjectMap.find (key);

        if ( itr != dataObjectMap.end() )
            itr->second;
        else
            return nullptr;
    }
    //Returns all data objects in a list
    const std::list<DataContainerType> getAllDataObjects()
    {
        std::list<DataContainerType> fullList;
        transform(dataObjectMap.begin(), dataObjectMap.end(), back_inserter(fullList), getSecond<DataContainerType>());

        return fullList;
    }

    //Returns objects selected by a criterion defined by the function parsed as input
    const std::list<DataContainerType> getObjByCriterion(bool (*exec)(const DataContainerType))
    {
        std::list<DataContainerType> listByCriterion;

        typename DataModelMap<DataContainerType>::const_iterator itr;
        for(itr = dataObjectMap.begin(); itr != dataObjectMap.end(); ++itr )
        {
            if(exec(itr->second))
                listByCriterion.push_back(itr->second);
        }

        return listByCriterion;
    }

    void clear() {
        dataObjectMap.clear();
    }

    friend class DataModelUpdater<DataContainerType>;
};





#endif


