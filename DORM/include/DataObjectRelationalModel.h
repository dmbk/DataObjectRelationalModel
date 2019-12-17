#ifndef DATAOBJECTRELATIONALMODEL_H
#define DATAOBJECTRELATIONALMODEL_H

#include "DataContainer.h"
#include "Util.h"
template <class DataContainerType>
struct Types {

    /**Usable-Models**/


    typedef DataModelUpdater<DataContainerType> DataModelUpdaterType;
    typedef typename DataModelUpdater<DataContainerType>::ConstIterType IterType;
    typedef std::list<DataContainerType> ListType;

};

template <class DataContainerType>
DataModelUpdater<DataContainerType>& initDataModel()
{

    static_assert(std::is_base_of<DataContainer, typename PlainType<DataContainerType>::type >::value, "Data type not inherited from 'DataContainer'");

    return DataModelUpdater<DataContainerType>::Instance();
}


#endif
