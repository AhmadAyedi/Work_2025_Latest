NO respect for LIN architecture 



we have 2 buttons each connected to one arduino our 2 slaves 
when we press on a button connected to slave1 we send a message to master :
hello from arduino1

and the same with button2 and arduino2 

here we didn't respect the LIN architecture 
normally the slave is not the one who initiate communication 
the accurate architecture and concept is that the master always send requests
(headers) for each slave to check if an event is triggered (an event occured or
take place or happen ) 
if yes then the master get that event and do something with it
soit y'affichih fil terminal te3 master (python shell) 
soit yib34o lil other slave to trigger some actuator connected to a slave or even
 to him