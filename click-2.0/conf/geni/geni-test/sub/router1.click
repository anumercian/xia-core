require(library ../../../xia_router_template_xtransport.click);
require(library ../../../xia_address.click);


// router instantiation
router1 :: Router(RE AD1 RHID1, AD1, RHID1);
//router1 :: Router4Port(RE AD1 RHID1);
//router1 :: Router4PortDummyCache(RE AD1 RHID1, AD1, RHID1); // if router does not understand CID pricipal


c0 :: Classifier(12/9999);
c1 :: Classifier(12/9999);
//c2 :: Classifier(12/9999);

todevice0 :: ToDevice(eth2);
todevice1 :: ToDevice(eth3);
//todevice2 :: ToDevice(eth0);

FromDevice(eth2, PROMISC true) 
//-> XIAPrint() 
-> c0;
c0[0] -> Strip(14) -> MarkXIAHeader() -> [0]router1; // XIA packet 


FromDevice(eth3, PROMISC true) 
//-> XIAPrint()
-> c1;
c1[0] -> Strip(14) -> MarkXIAHeader() -> [1]router1; // XIA packet 

//FromDevice(eth0, PROMISC true) -> c2;
//c2[0] -> Strip(14) -> MarkXIAHeader() -> [2]router1; // XIA packet 

//Idle -> [3]router1[3] -> Discard; 

router1[0]
//-> XIAPrint() 
-> EtherEncap(0x9999, 00:04:23:b7:3e:a2, 00:04:23:b7:13:1c) -> todevice0;

router1[1]
//-> XIAPrint() 
-> EtherEncap(0x9999, 00:04:23:b7:3e:a3, 00:04:23:b7:1a:be) 
-> c::XIAXIDTypeCounter(src AD, src HID, src SID, src CID, src IP, -)
-> todevice1; 


ControlSocket(tcp, 7777);


//router1[2]
//-> XIAPrint() 
//-> EtherEncap(0x9999, 00:03:47:73:b8:dc, 00:02:b3:35:e6:29) -> todevice2;


//Script(write gen.active true);  // the packet source should be activated after all other scripts are executed


//Script(write router1/n/proc/rt_HID/rt.add HID1 1);    
//Script(write router1/n/proc/rt_HID/rt.add HID2 2);  
//Script(write router1/n/proc/rt_HID/rt.add - 5);    

//Script(write router1/n/proc/rt_AD/rt.add AD0 0);     
//Script(write router1/n/proc/rt_AD/rt.add AD1 4);    
//Script(write router1/n/proc/rt_AD/rt.add - 5);   

//Script(write router1/n/proc/rt_SID/rt.add - 5);     // no default route for SID; consider other path
//Script(write router1/n/proc/rt_CID/rt.add - 5);     // no default route for CID; consider other path




