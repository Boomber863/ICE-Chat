#include <Ice/Ice.h>
#include <ChatI.h>
using namespace std;
using namespace Chat;

int main(int argc, char const *argv[])
{
  int exitStatus = EXIT_SUCCESS;  
  Ice::CommunicatorPtr ice;

  try
  {
    ice = Ice::initialize(argc, argv);
    Ice::ObjectAdapterPtr adapter = ice->createObjectAdapterWithEndpoints("adapter", "default -p 10000");
    Ice::ObjectPtr oServer = new ServerI;
    // Ice::ObjectPtr objectRoomFactory = new RoomFactoryI;

    adapter->add(oServer, Ice::stringToIdentity("Server"));
    // adapter->add(objectRoomFactory, ice->stringToIdentity("RoomFactory"));
    adapter->activate();

    cout << "Server is running..." << endl;

    ice->waitForShutdown();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
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
      std::cerr << e.what() << '\n';
      exitStatus = EXIT_FAILURE;
    }
  }

  return exitStatus;
}
