#include <Ice/Ice.h>
#include <ChatI.h>
using namespace std;
using namespace Chat;


int main(int argc, char const *argv[])
{
  Ice::CommunicatorPtr ice;
  int exitStatus = EXIT_SUCCESS;

  try
  {
    ice = Ice::initialize(argc, argv);
    Ice::ObjectAdapterPtr adapter = ice->createObjectAdapterWithEndpoints("adapter2", "default");

    string name;
    cout << "Enter your username: ";
    cin >> name;
    
    Ice::ObjectPtr oUser = new UserI(name);
    Ice::ObjectPrx userPrxBase = adapter->addWithUUID(oUser);
    adapter->activate();

    UserPrx userPrx = UserPrx::checkedCast(userPrxBase);

    Ice::ObjectPrx serverPrxBase = ice->stringToProxy("Server:default -h 10.1.1.35 -p 10000");
    // Ice::ObjectPrx roomFactoryPrxBase = ice->stringToProxy("RoomFactory:default -p 10000");

    ServerPrx serverPrx = ServerPrx::checkedCast(serverPrxBase);
    if(!serverPrx)
    {
      throw "Invalid server proxy";
    }

    // RoomFactoryPrx roomFactoryPrx = RoomFactoryPrx::checkedCast(roomFactoryPrxBase);
    // if(!roomFactoryPrx)
    // {
    //   throw "Invalid room factory proxy";
    // }

    serverPrx->logIn(userPrx);
    roomList rooms;

    bool end = false;
    while(!end)
    {
      fflush(stdin);

      cout << endl << endl;

      cout << "---------- Your username is: " << userPrx->getName() << " ------------" << endl;
      cout << "1 - create a room" << endl;
      cout << "2 - join a room" << endl;
      cout << "3 - leave a room" << endl;
      cout << "4 - send message to a room" << endl;
      cout << "5 - send direct message to an user" << endl;
      cout << "6 - remove a room" << endl;
      cout << "----------Rooms you're in------------" << endl;

      int roomCounter = 0;
      for (int i = 0; i < serverPrx->getRooms().size(); i++)
      {
        try
        {
          serverPrx->getRooms()[i]->getUser(userPrx->getName());
          roomCounter++;
          cout << "--> @" << serverPrx->getRooms()[i]->getName() << endl;
        }
        catch(UserDoesNotExist& e){}
      }

      if(roomCounter == 0)
      {
        cout << "You are not in any rooms." << endl;
      }
      cout << "--------------------------------" << endl;
      cout << "8 - exit" << endl;

      char action;
      cout << "Type action: ";
      cin >> action;

      //system("clear");

      switch (action)
      {
      case '1': // create a room
        try
        {
          string roomName;
          cout << "Name of room: ";
          cin >> roomName;

          RoomPrx proxy = serverPrx->addRoom(roomName, userPrx);

          cout << "Created the room!" << endl;
        }
        catch(RoomAlreadyExists& e){
					cout << "Room already exists!" << endl;
				}
        break;
      
      case '2': // Join the room
        try
        {
          string roomName;
          cout << "Name of room: ";
          cin >> roomName;

          RoomPrx proxy = NULL;

          proxy = serverPrx->getRoom(roomName);

          proxy->addUser(userPrx);

          cout << "You joined " << proxy->getName() << endl;

          rooms.push_back(proxy);
        }
        catch(RoomDoesNotExist& e){
					cout << "Room does not exist!" << endl;
				}
        catch(UserAlreadyExists& e){
					cout << "You are already a member of this room!" << endl;
				}
        break;

      case '3': // leave the room
        try
        {
          string roomName;
          cout << "Name of room: ";
          cin >> roomName;

          RoomPrx proxy = NULL;

          proxy = serverPrx->getRoom(roomName);

          proxy->removeUser(userPrx);
          cout << "You left the room!" << endl;
        }
        catch(RoomDoesNotExist& e){
					cout << "Room does not exist!" << endl;
				}
        catch(UserDoesNotExist& e){
					cout << "You are not a member of this room!" << endl;
				}
        break;

      case '4': // send message to the room
        try
        {
          char message[1024];
          string roomName;
          cout << "Room name: ";
          cin >> roomName;

          RoomPrx proxy = NULL;

          proxy = serverPrx->getRoom(roomName);

          cout << "Send: ";
          cin.ignore(1024, '\n');
          cin.getline(message, 1024);

          proxy->sendMessage(message, userPrx);
        }
        catch(RoomDoesNotExist& e){
					cout << "Room does not exist!" << endl;
				}
        catch(UserDoesNotExist& e){
					cout << "You are not a member of this room!" << endl;
				}
        break;

      case '5': // send private message
        try
        {
          char message[1024];
          string username;
          cout << "User you want to msg: " << endl;
          cin >> username;

          UserPrx proxy = NULL;
          proxy = serverPrx->getUser(username);

          cout << "Send: ";
          cin.ignore();
          cin.getline(message, 1024);

          proxy->receivePrivateMessage(message, userPrx);
        }
        catch(UserDoesNotExist& e){
						cout << "User does not exist!" << endl;
				}
        break;

      case '6': // remove the room
        try
        {
          string roomName;
          cout << "Room name: ";
          cin >> roomName;

          serverPrx->removeRoom(roomName, userPrx);

          cout << "Room was removed!" << endl;
        }
        catch(RoomDoesNotExist& e){
					cout << "Room does not exist!" << endl;
				}
        catch(UserDoesNotExist& e){
					cout << "You are not the owner of this room!" << endl;
				}
        
        break;

      case '8':
        serverPrx->logOut(userPrx);
        end = true;
        break;

      default:
        cout << "Unknown action!!!" << endl;
        break;
      }
    }

  }
  catch(UserAlreadyExists& e){
			cout << "User already exist" << endl;
			exitStatus = EXIT_FAILURE;
	}
  catch(const Ice::Exception& e)
  {
    std::cerr << e << '\n';
		exitStatus = EXIT_FAILURE;
  }
  
  if(ice)
  {
    try
    {
      ice->destroy();
    }
    catch(const std::exception& e)
    {
      cerr << e.what() << endl;
      exitStatus = EXIT_FAILURE;
    }
  }

  return exitStatus;
}
