module Chat
{

exception RoomDoesNotExist {};
exception RoomAlreadyExists {};
exception FactoryAlreadyExist {};
exception FactoryDoesNotExist {};
exception UserDoesNotExist {};
exception UserAlreadyExists {};

interface Room;
interface User;
// interface RoomFactory;

sequence<Room*> roomList;
sequence<User*> userList;

interface Server {
    void logIn(User* who) throws UserAlreadyExists;
    void logOut(User* who) throws UserDoesNotExist;
    User* getUser(string name) throws UserDoesNotExist;
    Room* getRoom(string name) throws RoomDoesNotExist;
    roomList getRooms();
    Room* addRoom(string name, User* owner) throws RoomAlreadyExists;
    void removeRoom(string name, User* owner) throws RoomDoesNotExist, UserDoesNotExist;
    // void addNewFactory(RoomFactory* factory) throws FactoryAlreadyExist;
    // void removeFactory(RoomFactory* factory) throws FactoryDoesNotExist;
  };

interface User {
    string getName();
    void receiveMessage(string message, User* sender, string roomName);
    void receivePrivateMessage(string message, User* sender);
  };

interface Room
{
  userList presentUsers();
  User* getUser(string name) throws UserDoesNotExist;
  void sendMessage(string message, User* sender) throws UserDoesNotExist;
  void addUser(User* who) throws UserAlreadyExists;
  void removeUser(User* who) throws UserDoesNotExist;
  string getName();
  string getOwnerName();
}

// interface RoomFactory {
//   Room* getRoom(string name);
//   roomList getRooms();
//   Room* createRoom(string name) throws RoomAlreadyExists;
//   void removeRoom(string name) throws RoomDoesNotExist;
// };


}
