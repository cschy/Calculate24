# Calculate24

A mini muti-players online game which rule is to calculate number 24 by 4 pokers.

# Environment
Use cocos2dx-3.17.2 game engine.

# make
step 1. Check and update your environment.

step 2. Delete all files from "src" and import all files of dir "Class" to here.

step 3. Replace your own server's IP and Port in \Client\Classes\Socket\SocketClient.cpp -> SocketClient::initNetwork(). Change the server Port in \Server\Game\24.cpp -> Calculate24::init()
