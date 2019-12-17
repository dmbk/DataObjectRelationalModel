#ifndef USER_H
#define USER_H
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>
#include <sstream>
#include "DataContainer.h"

/***This file contains all user class definitions***/

enum UserType
{
    NONE = 0,
    ADMIN_USER,
    OPERATOR_USER,
    GUEST_USER
};

struct PersonalInfo
{

    std::string name;
    std::string dob;
    std::string phoneNumber;

private:
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & name;
        ar & dob;
        ar & phoneNumber;
    }


};

//Must iherit from DataContainer, if not static compile time check will throw error
class AbstractUser : public DataContainer
{
public:
    //Common attributes to users
    unsigned int userId;
    std::string userName;
    UserType type;
    std::string roleDescription;
    PersonalInfo personalInfo;


    AbstractUser(unsigned int id, std::string userName, PersonalInfo info):DataContainer(id, std::to_string(id)), userId(id), userName(userName), personalInfo(info) {}
    AbstractUser(unsigned int id, std::string userName):DataContainer(id, std::to_string(id)), userId(id), userName(userName) {}

    AbstractUser() {}

    std::string getUserName()const {
        return userName;
    }
    unsigned int getUserId()const {
        return userId;
    }
    UserType getUserType()const {
        return type;
    }
    std::string getUserRoleDescription()const {
        return roleDescription;
    }
    PersonalInfo getPersonalInfo()const {
        return personalInfo;
    }

private:
    //Serialization support
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<DataContainer>(*this);
        ar & userName;
        ar & type;
        ar & roleDescription;
        ar & personalInfo;
    }


};

/*
*Below classes are the definitions of concrete level users
*/
class AdminUser: public AbstractUser
{
public:
    AdminUser(unsigned int id, std::string userName, PersonalInfo info): AbstractUser(id, userName, info) {
        type = ADMIN_USER;
        roleDescription = "Administration";
    }
    AdminUser(unsigned int id, std::string userName): AbstractUser(id, userName) {
        type = ADMIN_USER;
        roleDescription = "Administration";
    }

    AdminUser(): AbstractUser() {
        type = ADMIN_USER;
        roleDescription = "Administration";
    }
private:
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<AbstractUser>(*this);

    }

};


class OperatorUser: public AbstractUser
{
public:
    OperatorUser(unsigned int id, std::string userName, PersonalInfo info): AbstractUser(id, userName, info) {
        type = OPERATOR_USER;
        roleDescription = "Operations";
    }
    OperatorUser(unsigned int id, std::string userName): AbstractUser(id, userName) {
        type = OPERATOR_USER;
        roleDescription = "Operations";
    }

    OperatorUser(): AbstractUser() {
        type = OPERATOR_USER;
        roleDescription = "Operations";
    }
private:
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<AbstractUser>(*this);

    }

};

class GuestUser: public AbstractUser
{
public:
    GuestUser(unsigned int id, std::string userName, PersonalInfo info): AbstractUser(id, userName, info) {
        type = GUEST_USER;
        roleDescription = "Visiting";
    }
    GuestUser(unsigned int id, std::string userName): AbstractUser(id, userName) {
        type = GUEST_USER;
        roleDescription = "Visiting";
    }

    GuestUser() : AbstractUser() {
        type = GUEST_USER;
        roleDescription = "Visiting";
    }
private:
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<AbstractUser>(*this);

    }

};

#endif
