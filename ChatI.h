#ifndef CHAT_I_H
#define CHAT_I_H

#include <Chat.h>
#include <Ice/ObjectAdapter.h>


namespace Chat
{
  
  class ServerI : virtual public Server
  {

    public:
      userList inactiveUsersList;
      roomList rooms;

      ServerI();

      virtual void logIn(const ::Chat::UserPrx&, const Ice::Current&);

      virtual void logOut(const ::Chat::UserPrx&, const Ice::Current&);

      virtual ::Chat::roomList getRooms(const Ice::Current&);

      virtual ::Chat::RoomPrx getRoom(const ::std::string&, const Ice::Current&);

      virtual ::Chat::UserPrx getUser(const ::std::string&, const Ice::Current&);

      virtual ::Chat::RoomPrx addRoom(const ::std::string&, const ::Chat::UserPrx&, const Ice::Current&);

      virtual void removeRoom(const ::std::string&, const ::Chat::UserPrx&, const Ice::Current&);

      // virtual void addNewFactory(const ::Chat::RoomFactoryPrx&, const Ice::Current&);

      // virtual void removeFactory(const ::Chat::RoomFactoryPrx&, const Ice::Current&);

  };


  class RoomI : virtual public Room
  {

    public:
      userList users;
      std::string name;
      std::string ownerName;

      RoomI(const std::string&, const ::Chat::UserPrx&);

      virtual ::Chat::userList presentUsers(const Ice::Current&);

      virtual void sendMessage(const ::std::string&, const ::Chat::UserPrx&, const Ice::Current&);

      virtual ::Chat::UserPrx getUser(const ::std::string&, const Ice::Current&);

      virtual void addUser(const ::Chat::UserPrx&, const Ice::Current&);

      virtual void removeUser(const ::Chat::UserPrx&, const Ice::Current&);

      virtual ::std::string getName(const Ice::Current&);

      virtual ::std::string getOwnerName(const Ice::Current&);

  };


  // class RoomFactoryI : virtual public RoomFactory
  // {
  //   public:
  //     roomList rooms;

  //     RoomFactoryI();

  //     virtual ::Chat::RoomPrx getRoom(const ::std::string&, const Ice::Current&);

  //     virtual ::Chat::roomList getRooms(const Ice::Current&);

  //     virtual ::Chat::RoomPrx createRoom(const ::std::string&, const Ice::Current&);

  //     virtual void removeRoom(const ::std::string&, const Ice::Current&);

  // };


  class UserI : virtual public User
  {
    public:
      std::string name;

      UserI(const ::std::string&);

      virtual ::std::string getName(const Ice::Current&);

      virtual void receiveMessage(const ::std::string&, const ::Chat::UserPrx&, const ::std::string&, const Ice::Current&);

      virtual void receivePrivateMessage(const ::std::string&, const ::Chat::UserPrx&, const Ice::Current&);

  };

} // namespace Chat



#endif