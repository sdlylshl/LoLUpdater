[![Visit our IRC channel](https://kiwiirc.com/buttons/irc.quakenet.org/LoLUpdater.png)](https://kiwiirc.com/client/irc.quakenet.org/#LoLUpdater)

[Website](http://LoLUpdater.com)

Program Instructions (Humanly readable):

1. Download Nvidia cg from official http (wait for DL), install using "/verysilent /TYPE = compact", wait for process to exit
2. Concentrate a bunch of strings (paths to be used for copying later)
3. Download Adobe Air (beta) from offical https (wait for DL), install using "-silent", wait for process to exit
4. Concentrate more strings
5. Check if Garena LoL, replace value of current strings with the ones appropriate for Garena
6. check for XP, if true then download file to location
7. Check if AVX, if true then download to location
8. check if SSE2, if true the download to location
9. check if SSE, if true download to location
10. else download "default" file to location
11. Copy all files from NvidiaCG/AdobeAIR install dir to location
12. Unblock files using DeleteFile (pathtofile + ":Zone.Identifier")

13. Die of happiness