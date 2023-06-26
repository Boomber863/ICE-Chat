#include <ChatI.h>

// ServerI implementation

Chat::ServerI::ServerI() {
  roomList rooms;
  userList inactiveUsersList;
}

void Chat::ServerI::logIn(const ::Chat::UserPrx& who, const Ice::Current& current) 
{
  for (int i = 0; i < inactiveUsersList.size(); i++)
  {
    if(inactiveUsersList[i]->getName().compare(who->getName()) == 0)
    {
      throw Chat::UserAlreadyExists(); 
    }
  }

  inactiveUsersList.push_back(who);

  std::cout << "User: " << who->getName() << " logged in" << std::endl;
}


void Chat::ServerI::logOut(const ::Chat::UserPrx& who, const Ice::Current& current)
{
  for (int i = 0; i < inactiveUsersList.size(); i++)
  {
    if(inactiveUsersList[i]->getName().compare(who->getName()) == 0)
    {

      for (int j = 0; j < rooms.size(); j++)
      {
        if(rooms[j]->getOwnerName().compare(who->getName()) == 0){
          rooms.erase(rooms.begin() + j);
        }
      }

      inactiveUsersList.erase(inactiveUsersList.begin() + i);
      return;      
    }
  }
}


::Chat::roomList Chat::ServerI::getRooms(const Ice::Current& current)
{
  return rooms;
}


::Chat::RoomPrx Chat::ServerI::getRoom(const ::std::string& name, const Ice::Current& current)
{
  for (int i = 0; i < rooms.size(); i++)
  {
    if(rooms[i]->getName().compare(name) == 0)
    {
      return rooms[i];
    }
  }

  throw Chat::RoomDoesNotExist();
}

::Chat::UserPrx Chat::ServerI::getUser(const ::std::string& name, const Ice::Current& current)
{

  for (int i = 0; i < inactiveUsersList.size(); i++)
  {
    if(inactiveUsersList[i]->getName().compare(name) == 0)
    {
      return inactiveUsersList[i];
    }
  }

  throw ::Chat::UserDoesNotExist();
}


::Chat::RoomPrx Chat::ServerI::addRoom(const ::std::string& name, const ::Chat::UserPrx& owner, const Ice::Current& current)
{
  for (int i = 0; i < rooms.size(); i++)
  {
    if(rooms[i]->getName().compare(name) == 0)
    {
      throw Chat::RoomAlreadyExists();
    }
  }
  
  RoomPtr servant = new RoomI(name, owner);
  RoomPrx newRoom  = RoomPrx::uncheckedCast(current.adapter->addWithUUID(servant));

  rooms.push_back(newRoom);
  return newRoom;
}


void Chat::ServerI::removeRoom(const ::std::string& name, const ::Chat::UserPrx& owner, const Ice::Current& current)
{
  for (int i = 0; i < rooms.size(); i++)
  {
    if(rooms[i]->getName().compare(name) == 0)
    {
      if(rooms[i]->getOwnerName().compare(owner->getName()) != 0) {
        throw Chat::UserDoesNotExist();
      }

      rooms.erase(rooms.begin() + i);
      return;
    }
  }

  throw Chat::RoomDoesNotExist();
}


// void Chat::ServerI::addNewFactory(const ::Chat::RoomFactoryPrx& roomFactory, const Ice::Current& current)
// {
//   rooms.push_back(roomFactory->getRooms().back());
// }


// void Chat::ServerI::removeFactory(const ::Chat::RoomFactoryPrx& factory, const Ice::Current& current)
// {

// }




// RoomI implementation

Chat::RoomI::RoomI(const std::string& pName, const ::Chat::UserPrx& owner)
{
  name = pName;
  ownerName = owner->getName();
}


::Chat::userList Chat::RoomI::presentUsers(const Ice::Current& current)
{
  return users;
}


void Chat::RoomI::sendMessage(const ::std::string& message, const ::Chat::UserPrx& sender, const Ice::Current& current)
{

  bool isMember = false;
  for (int i = 0; i < users.size(); i++)
  {
    if(users[i]->getName().compare(sender->getName()) == 0)
    {
      isMember = true;
      break;
    }
  }

  if(!isMember) {
    throw Chat::UserDoesNotExist();
  }

  for (int i = 0; i < users.size(); i++)
  {
    if(users[i]->getName().compare(sender->getName()) != 0)
    {
      users[i]->receiveMessage(message, sender, this->getName(current));
    }
  }
  
}

::Chat::UserPrx Chat::RoomI::getUser(const ::std::string& name, const Ice::Current& current)
{
    for (int i = 0; i < users.size(); i++)
  {
    if(users[i]->getName().compare(name) == 0)
    {
      return users[i];
    }
  }

  throw ::Chat::UserDoesNotExist();
}

void Chat::RoomI::addUser(const ::Chat::UserPrx& who, const Ice::Current& current)
{
  for (int i = 0; i < users.size(); i++)
  {
    if(users[i]->getName().compare(who->getName()) == 0)
    {
      throw Chat::UserAlreadyExists();
    }
  }
  
  users.push_back(who);
}


void Chat::RoomI::removeUser(const ::Chat::UserPrx& who, const Ice::Current& current)
{
  for (int i = 0; i < users.size(); i++)
  {
    if(users[i]->getName().compare(who->getName()) == 0)
    {
      users.erase(users.begin() + i);
      return;
    }
  }

  throw Chat::UserDoesNotExist();
}


::std::string Chat::RoomI::getName(const Ice::Current& current)
{
  return name;
}

::std::string Chat::RoomI::getOwnerName(const Ice::Current& current)
{
  return ownerName;
}



// RoomFactoryI implementation

// Chat::RoomFactoryI::RoomFactoryI()
// {
//   roomList rooms;
// }


// ::Chat::RoomPrx Chat::RoomFactoryI::getRoom(const ::std::string& name, const Ice::Current& current)
// {
//   for (int i = 0; i < rooms.size(); i++)
//   {
//     if(rooms[i]->getName().compare(name) == 0)
//     {
//       return rooms[i];
//     }
//   }

//   return NULL;
// }


// ::Chat::roomList Chat::RoomFactoryI::getRooms(const Ice::Current& current)
// {
//   return rooms;
// }


// ::Chat::RoomPrx Chat::RoomFactoryI::createRoom(const ::std::string& name, const Ice::Current& current)
// {
//   for (int i = 0; i < rooms.size(); i++)
//   {
//     if(rooms[i]->getName().compare(name) == 0)
//     {
//       throw Chat::RoomAlreadyExists();
//     }
//   }
  
//   RoomPtr servant = new RoomI(name);
//   RoomPrx newRoom  = RoomPrx::uncheckedCast(current.adapter->addWithUUID(servant));

//   rooms.push_back(newRoom);
// }


// void Chat::RoomFactoryI::removeRoom(const ::std::string& name, const Ice::Current& current)
// {
//   for (int i = 0; i < rooms.size(); i++)
//   {
//     if(rooms[i]->getName().compare(name) == 0)
//     {
//       rooms.erase(rooms.begin() + i);
//       return;
//     }
//   }

//   throw Chat::RoomDoesNotExist();
// }





// UserI implementation

Chat::UserI::UserI(const ::std::string& pName)
{
  name = pName;
}

::std::string Chat::UserI::getName(const Ice::Current& current)
{
  return name;
}


void Chat::UserI::receiveMessage(const ::std::string& message, const ::Chat::UserPrx& sender, const ::std::string& roomName, const Ice::Current& current)
{
    std::cout << std::endl <<sender->getName() + "@" + roomName + " : " + message << std::endl;
}


void Chat::UserI::receivePrivateMessage(const ::std::string& message, const ::Chat::UserPrx& sender, const Ice::Current& current)
{
    std::cout << std::endl << "private message from " + sender->getName() + " : " + message << std::endl;
}