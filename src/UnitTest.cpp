#define BOOST_TEST_MODULE main_functionality_check
#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/list.hpp>
#include "User.h"
#include "DataObjectRelationalModel.h"
#include <typeindex>
#include <typeinfo>
typedef boost::mpl::list<AdminUser,OperatorUser,GuestUser> test_types;

/***This file contains the test cases to check functionality***/

//Checks if a user can be successfully fetched using id
BOOST_AUTO_TEST_CASE_TEMPLATE( test_getElement, T, test_types )
{
    T* user = new T(static_cast<int>(typeid(T).hash_code()), std::string("user_test_getElement"), {std::string("Dulitha"), std::string("24/12/1991"), std::string("+94702337343")});
    DataModelUpdater<AbstractUser*>& db = initDataModel<AbstractUser*>();
    db.insertElement(user);
    int i;
    Types<AbstractUser*>::IterType userFetched = db.getElement(static_cast<int>(typeid(T).hash_code()),i);

    UserType type = NONE;
    if(typeid(T).hash_code() == typeid(AdminUser).hash_code())
        type = ADMIN_USER;
    else if(typeid(T).hash_code() == typeid(OperatorUser).hash_code())
        type = OPERATOR_USER;
    else if(typeid(T).hash_code() == typeid(GuestUser).hash_code())
        type = GUEST_USER;

    BOOST_CHECK((i == 1) && ( type == static_cast <AbstractUser*>(userFetched->second)->getUserType()) );

    db.clearElements();
    delete user;

}

//Checks if all the users are returned
BOOST_AUTO_TEST_CASE(test_getAllElements)
{
    DataModelUpdater<AbstractUser*>& db = initDataModel<AbstractUser*>();
    AdminUser* admin_1  = new AdminUser(4, std::string("admin_1"), {std::string("Dulitha"), std::string("24/12/1991"), std::string("+94702337343")});
    OperatorUser* operator_1 = new OperatorUser(5,std::string("operator_1"), {std::string("Christina"), std::string("28/02/1990"), std::string("+94702337342")});
    GuestUser* guest_1  = new GuestUser(6, std::string("guest"), {std::string("User1"), std::string("24/02/1991"), std::string("+94762335343")});
    GuestUser* guest_2 = new GuestUser(7,std::string("guest"), {std::string("User2"), std::string("21/02/1990"), std::string("+94762347342")});

    db.insertElement(admin_1);
    db.insertElement(operator_1);
    db.insertElement(guest_1);
    db.insertElement(guest_2);

    Types<AbstractUser*>::ListType all_list= db.getAllElements();

    BOOST_CHECK_EQUAL(4, all_list.size());

    db.clearElements();
    delete admin_1;
    delete operator_1;
    delete guest_1;
    delete guest_2;

}


bool exec(AbstractUser* dataObject) {
    PersonalInfo pInfo =  dataObject->getPersonalInfo();
    std::string mobile = pInfo.phoneNumber;
    if(mobile.rfind("+9476", 0) != std::string::npos)
    {
        return true;
    }
    return false;
}

//Checks if the users returned are as per the criterion
BOOST_AUTO_TEST_CASE(test_getElementsByCriterion)
{
    DataModelUpdater<AbstractUser*>& db = initDataModel<AbstractUser*>();
    AdminUser* admin_1  = new AdminUser(4, std::string("admin_1"), {std::string("Dulitha"), std::string("24/12/1991"), std::string("+94702337343")});
    OperatorUser* operator_1 = new OperatorUser(5,std::string("operator_1"), {std::string("Christina"), std::string("28/02/1990"), std::string("+94702337342")});
    GuestUser* guest_1  = new GuestUser(6, std::string("guest"), {std::string("User1"), std::string("24/02/1991"), std::string("+94762335343")});
    GuestUser* guest_2 = new GuestUser(7,std::string("guest"), {std::string("User2"), std::string("21/02/1990"), std::string("+94762347342")});

    db.insertElement(admin_1);
    db.insertElement(operator_1);
    db.insertElement(guest_1);
    db.insertElement(guest_2);
    Types<AbstractUser*>::ListType::iterator it;
    Types<AbstractUser*>::ListType dialogUsers = db.getElementsByCriterion(exec);
    BOOST_CHECK_EQUAL(dialogUsers.size(), 2);
    for(it = dialogUsers.begin(); it != dialogUsers.end(); ++it)
    {
        BOOST_CHECK_EQUAL((*it)->getUserName(), "guest");
    }

    db.clearElements();
    delete admin_1;
    delete operator_1;
    delete guest_1;
    delete guest_2;

}


//Checks serialization
BOOST_AUTO_TEST_CASE(test_serialization)
{
    DataModelUpdater<AbstractUser*>& db = initDataModel<AbstractUser*>();
    AdminUser* admin_1  = new AdminUser(4, std::string("admin_1"), {std::string("Dulitha"), std::string("24/12/1991"), std::string("+94702337343")});
    OperatorUser* operator_1 = new OperatorUser(5,std::string("operator_1"), {std::string("Christina"), std::string("28/02/1990"), std::string("+94702337342")});
    GuestUser* guest_1  = new GuestUser(6, std::string("guest"), {std::string("User1"), std::string("24/02/1991"), std::string("+94762335343")});
    GuestUser* guest_2 = new GuestUser(7,std::string("guest"), {std::string("User2"), std::string("21/02/1990"), std::string("+94762347342")});

    db.insertElement(admin_1);
    db.insertElement(operator_1);
    db.insertElement(guest_1);
    db.insertElement(guest_2);

    db.saveElements();
    BOOST_CHECK_EQUAL(db.getAllElements().size(),0);
    db.restoreElements();
    BOOST_CHECK_EQUAL(db.getAllElements().size(),4);

    db.clearElements();
    delete admin_1;
    delete operator_1;
    delete guest_1;
    delete guest_2;

}

void modify(AbstractUser*& dataObject)
{
    dataObject->userName = "Kularathne";
}

//Checks modifyElement
BOOST_AUTO_TEST_CASE(test_modifyElement)
{
    DataModelUpdater<AbstractUser*>& db = initDataModel<AbstractUser*>();
    AdminUser* admin_1  = new AdminUser(4, std::string("admin_1"), {std::string("Dulitha"), std::string("24/12/1991"), std::string("+94702337343")});
    OperatorUser* operator_1 = new OperatorUser(5,std::string("operator_1"), {std::string("Christina"), std::string("28/02/1990"), std::string("+94702337342")});
    GuestUser* guest_1  = new GuestUser(6, std::string("guest"), {std::string("User1"), std::string("24/02/1991"), std::string("+94762335343")});
    GuestUser* guest_2 = new GuestUser(7,std::string("guest"), {std::string("User2"), std::string("21/02/1990"), std::string("+94762347342")});

    db.insertElement(admin_1);
    db.insertElement(operator_1);
    db.insertElement(guest_1);
    db.insertElement(guest_2);

    if(db.modifyElement(4, modify))
    {
        int i;
        Types<AbstractUser*>::IterType userFetched = db.getElement(4,i);
        BOOST_CHECK((i == 1) && ( std::string("Kularathne") == static_cast <AbstractUser*>(userFetched->second)->getUserName()) );

    }
    else
        BOOST_CHECK(false);
    db.clearElements();
    delete admin_1;
    delete operator_1;
    delete guest_1;
    delete guest_2;

}



//Checks deleteElement
BOOST_AUTO_TEST_CASE(test_deleteElement)
{
    DataModelUpdater<AbstractUser*>& db = initDataModel<AbstractUser*>();
    AdminUser* admin_1  = new AdminUser(4, std::string("admin_1"), {std::string("Dulitha"), std::string("24/12/1991"), std::string("+94702337343")});
    OperatorUser* operator_1 = new OperatorUser(5,std::string("operator_1"), {std::string("Christina"), std::string("28/02/1990"), std::string("+94702337342")});
    GuestUser* guest_1  = new GuestUser(6, std::string("guest"), {std::string("User1"), std::string("24/02/1991"), std::string("+94762335343")});
    GuestUser* guest_2 = new GuestUser(7,std::string("guest"), {std::string("User2"), std::string("21/02/1990"), std::string("+94762347342")});

    db.insertElement(admin_1);
    db.insertElement(operator_1);
    db.insertElement(guest_1);
    db.insertElement(guest_2);

    BOOST_CHECK_EQUAL(db.getAllElements().size(),4);
    db.deleteElement(5);
    BOOST_CHECK_EQUAL(db.getAllElements().size(),3);

    db.clearElements();
    delete admin_1;
    delete operator_1;
    delete guest_1;
    delete guest_2;

}

