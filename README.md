# Note about Ashita3/Ashita4 compatibility
Multisend has not been rewritten in any meaningful capacity for Ashita4.  While the code is hard to read, it should still be functional and compatible with Ashita3 instances running Multisend of the same version for Ashita3.  This decision was made to ensure compatibilty during the transition period between Ashita3 and Ashita4.  Once Ashita4 has completely replaced Ashita3, Multisend will receive a rewrite as my other plugins have to make the codebase more readable and potentially add new features.  At that point, you will no longer be able to use Ashita3 Multisend alongside Ashita4 Multisend.

# Multisend
Multisend is a replacement for servo.  No synchronization is needed, just load and go.<br>
Each character receiving or sending data must be on Ashita3 or Ashita4 and have a compatible version of Multisend loaded.<br>
The current version of Multisend is 1.17 for both Ashita3 and Ashita4.<br>

**/ms send [command]**<br>
**/mss [command]**<br>
Sends a command to all characters.<br>

**/ms sendto [character] [command]**<br>
**/mst [character] [command]**<br>
Sends a command to a specific character.<br>

**/ms sendgroup [group] [command]**<br>
**/msg [group] [command]**<br>
Sends a command to all characters listed in a group.  Groups can be defined in Ashita/Config/MultiSend.xml.  See Ashita/docs/multisend/XML Structure file for example.<br>

**/ms sendothers [command]**<br>
**/mso [command]**<br>
Sends a command to all characters except for the current character.<br>

**/ms sendparty [command]**<br>
**/msp [command]**<br>
Sends a command to all characters in the current character's party.  This includes the current character unless ignoreself is turned on.<br>

**/ms sendally [command]**<br>
**/msa [command]**<br>
Sends a command to all characters in the current character's alliance.  This includes the current character unless ignoreself is turned on.<br>

**/ms followmax [#]**<br>
Sets a max follow range(in yalms).  If follow target is more than this many yalms away, following characters won't move.<br>
-I recommend setting between 30 and 100 to prevent following from trying to use portals.<br>
-Default is 5000, which will be the same behavior as before this setting was added.<br>

**/ms followme [on/off]**<br>
Enables or disables followme.  When followme is enabled, any characters with follow enabled will follow the player who last activated followme.  Default: OFF.<br>
-Followme can only be enabled by one character.  If a second character turns it on, the first character will have it disabled automatically.<br>

**/ms follow [on/off]**<br>
Enables or disables follow.  Any character with follow enabled with follow the player who last activated followme.  Default: ON<br>

**/ms attemptzone on/off**<br>
Enables or disables attempted zoning.  When enabled, if the leader zones, the other characters will continue running in the direction they were last running.  Default: OFF<br>
-If you are using a home point, survival guide, etc, it will be detected as long as you stand still throughout the process and nobody will run off.<br>
-BUT, if you move after first acting upon the object or sending an uberwarp command, chars may run off.<br>

**/ms ignoreself on/off**<br>
When enabled, any command that would effect the local character is not executed by the local character. Default: OFF<br>

**/ms debug on/off**<br>
When enabled, all received commands are printed to log and any character with followme updated will spam log with position updates.  Default: OFF<br>

**/ms reload**<br>
Typing this will reload your groups xml.<br>

**/ms help**<br>
Typing this will print a list of commands ingame.<br>

# Special Targeting
You may use **[t]** **[me]** or **[p0] [p1] [p2] [p3] [p4] [p5]** to replace the <> equivalents, if you wish to resolve them on local character.<br>
This will result in the *sending* character's data being used.<br>
Using **<t>** **<me>** etc.. will result in the *receiving* character's data being used.<br>
For example, if CharacterA uses */ms sendto CharacterB /ma "Fire IV" [t]*, then CharacterB will cast Fire IV on CharacterA's target.<br>
However, if CharacterA uses */ms sendto CharacterB /ma "Fire IV" <t>*, then CharacterB will cast Fire IV on whatever CharacterB has targeted at the time.
