#ifndef DATACONTAINER_H
#define DATACONTAINER_H
#include <string>
using std::string;
#include "DataModel.h"
#include "Util.h"
#include <list>
#include <sstream>
#include <fstream>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/unordered_map.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

/*****************************************************************
 * Class Name      : DataContainer
 *
 * Base Class      : None
 *
 * Purpose         : Used as the container object of the DataModel
 *
 * Special Members : id - unique object id
 * hashable - hashable string
 * templates       :
 * DataObjType - None
 *
 ****************************************************************/
class DataContainer
{
public:
    unsigned int id;
    std::string hashable;
    DataContainer(unsigned int uniqueId = 0, std::string hashableString = ""):id(uniqueId), hashable(hashableString) {}

    operator std::string()
    {
        return hashable;
    }

    operator  int()
    {
        return id;
    }
private:
    //Serialization support
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & id;
        ar & hashable;
    }

};

/*****************************************************************
 * Class Name      : DataModelUpdater
 *
 * Base Class      : Singleton
 *
 * Purpose         : Acts as the main interfacing object between DataModel and User
 *
 * Special Members : None
 * templates       :
 * DataContainerType - The object type assigned by the user to be stored in the underlying map
 *
 ****************************************************************/
template <class DataContainerType>
class DataModelUpdater: public Singleton<DataModelUpdater<DataContainerType> >
{
    DataModel<DataContainerType>& dataModel;
    friend class Singleton<DataModelUpdater<DataContainerType> >;

private:
    DataModelUpdater(): dataModel(DataModel<DataContainerType>::Instance()) {}
    ~DataModelUpdater() {}

public:

    typedef typename DataModelMap<DataContainerType>::const_iterator ConstIterType;
    typedef typename DataModelMap<DataContainerType>::iterator IterType;




    //Inserts a single element to the DataModel
    void insertElement(DataContainerType value) {
        dataModel.dataObjectMap.insert(std::make_pair<Key<DataContainerType>, DataContainerType>(Key<DataContainerType>(value), (DataContainerType)value));
    }

    //Returns an const_iterator to the element refered by unique id
    ConstIterType getElement(unsigned int id, int& i)
    {
        typedef typename PlainType<DataContainerType>::type PlainReturnType;

        Key<DataContainerType> searchKey;
        PlainReturnType rtypeObj(id, std::to_string(id));
        searchKey.template setVals<PlainReturnType>(rtypeObj);
        ConstIterType it = dataModel.dataObjectMap.find(searchKey);

        if(dataModel.dataObjectMap.end() != it)
            i = 1;
        else i = 0;

        return it;

    }

    //Returns a list containing all the elements in DataModel
    const std::list<DataContainerType> getAllElements()
    {
        return dataModel.getAllDataObjects();
    }

    //Returns a list containing the selected elements from DataModel: criterion by exec function
    const std::list<DataContainerType> getElementsByCriterion( bool (*exec)(const DataContainerType))
    {
        return dataModel.getObjByCriterion(exec);
    }

    //Clear DataModel
    void clearElements()
    {
        dataModel.clear();
    }

    //Only way to apply modifications to the element
    bool modifyElement(unsigned int id, void (*modify)(DataContainerType&))
    {
        typedef typename PlainType<DataContainerType>::type PlainReturnType;

        Key<DataContainerType> searchKey;
        PlainReturnType rtypeObj(id, std::to_string(id));
        searchKey.template setVals<PlainReturnType>(rtypeObj);
        IterType it = dataModel.dataObjectMap.find(searchKey);

        if(dataModel.dataObjectMap.end() != it)
        {
            modify(it->second);
            return true;
        }
        return false;
    }

    //Deletes single element given id
    IterType deleteElement(unsigned int id )
    {
        typedef typename PlainType<DataContainerType>::type PlainReturnType;

        Key<DataContainerType> searchKey;
        PlainReturnType rtypeObj(id, std::to_string(id));
        searchKey.template setVals<PlainReturnType>(rtypeObj);
        IterType it = dataModel.dataObjectMap.find(searchKey);

        if(dataModel.dataObjectMap.end() != it)
        {
            it = dataModel.dataObjectMap.erase(it);

            return it;
        }
        return dataModel.dataObjectMap.end();

    }

    //Serialize elements in DataModel
    void saveElements()
    {

        std::ofstream ofs("om.txt");
        if(ofs.is_open()) {
            boost::archive::text_oarchive oa(ofs);
            oa << dataModel.dataObjectMap;
            ofs.close();
            clearElements();
        }

    }

    //Restore elements in DataModel
    void restoreElements()
    {

        DataModelMap<DataContainerType> restored;
        std::ifstream ifs("om.txt");
        if(ifs.is_open())
        {
            boost::archive::text_iarchive ia(ifs);
            ia >> restored;
            ifs.close();
        }
        dataModel.dataObjectMap = restored;

    }

};

#endif
