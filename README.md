# Note about MultiSend and Ashita4
Multisend has not been rewritten in any meaningful capacity for Ashita4.  While the code is hard to read, it should still be functional and compatible with Ashita3 instances running Multisend 1.17 for Ashita3.  This decision was made to ensure compatibilty during the transition period between Ashita3 and Ashita4.  Once Ashita4 has completely replaced Ashita3, MultiSend will receive a rewrite as my other plugins have to make the codebase more readable and potentially add new features.  At this point, you will no longer be able to use Ashita3 MultiSend alongside Ashita4 MultiSend.

# MultiSend
Multisend is a replacement for servo.  No synchronization is needed, just load and go.
Commands are as follows(all can be prefixed with /ms or /multisend):


/ms send [command] - Sends the command to all characters with MultiSend loaded.

/ms sendto CharName [command] - Sends the command to a specific character, if they have MultiSend loaded.

/ms sendgroup GroupName [command] - Sends the command to all characters listed in a group.  Groups can be defined in Ashita/Config/MultiSend.xml.  See XML Structure file for example.

/ms followme on/off - Enables or disables followme.  When followme is enabled, any characters with follow enabled will follow the player who last activated followme.

/ms follow on/off - Enables or disables follow.  Any character with follow enabled with follow the player who last activated followme.

/ms ignoreself on/off - When enabled, any command that would effect the local character is not executed by the local character.

/ms debug on/off - When enabled, all received commands are printed to log and any character with followme updated will spam log with position updates.  For debug purposes, obviously.

/ms safemode on/off - When enabled, '/echo' will be inserted for commands missing the leading '/' to prevent chatlog spam

/ms reload - Will reload groups xml.

/ms help - Print an ingame reference.


You may use [t] [me] or [p0] [p1] [p2] [p3] [p4] [p5] to replace the <> equivalents, if you wish to resolve them on local character.  This will replace them with the appropriate server IDs, allowing the command to resolve on receiving character.


Example: Typing /ms send /ma "Fire IV" [t] will make all characters with multisend loaded cast Fire IV on the character who typed the command's target, rather than their own current target.