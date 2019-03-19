# IRCServerv2
IRC WIP

Main starts IRC

IRC spawns socket threads and start listening.

Acceptor thread directs connections to each socket thread.

(WIP: add authenticator to IRC)

Socket threads will listen and send incoming messages into queues.

Message handler thread checks each socket thread for queues, (WIP:sends to main chat log), and broadcasts the new message out.

IRC has an interactable console for issueing messages/commands. 

Could potentially create more IRC instances with their own chat channels. Would need little effort to do so, but movement of interactable console is necessary up to main.
