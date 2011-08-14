define($COUNT 100000000);
define($IP_RT_SIZE 351611);
define($IP_RANDOMIZE_MAX_CYCLE $IP_RT_SIZE);
define($IP6_RT_SIZE $IP_RT_SIZE);
define($IP6_RANDOMIZE_MAX_CYCLE $IP_RT_SIZE);
//define($AD_RT_SIZE 351611);
//define($CID_RT_SIZE 10);
define($AD_RANDOMIZE_MAX_CYCLE $AD_RT_SIZE);
define($CID_RANDOMIZE_MAX_CYCLE $CID_RT_SIZE);
define($PAYLOAD_SIZE 64);
define($HEADROOM_SIZE 148);

AddressInfo(
    RANDOM_IP 0.0.0.0,
    UNROUTABLE_IP 192.168.10.10,
    RANDOM_IP6 0::0,
    UNROUTABLE_IP6 1234::5678,
);

XIAXIDInfo(
    RANDOM_ID 1:0000000000000000000000000000000000000000,
    ARB_RANDOM_ID 2:0000000000000000000000000000000000000000,
    UNROUTABLE_AD0 AD:1000000000000000000000000000000000000009,
    UNROUTABLE_AD1 AD:2000000000000000000000000000000000000008,
    UNROUTABLE_AD2 AD:3000000000000000000000000000000000000007,
    SELF_AD AD:5500000000000000000000000000000000000055,
    CID0 CID:000102030405060708090a0b0c0d0e0f10111213,
);

elementclass IPPacketForward {
    host0 :: IPRouter4Port(192.168.0.1);

    //Script(write host0/rt.add 192.168.0.0/24 0);
    //Script(write host0/rt.add 0.0.0.0/0.0.0.0 1);
    Script(print_usertime, write host0/rt.load ip_routes.txt, print_usertime);

    input[0]
    -> Clone($COUNT)
    -> Unqueue
    -> IPRandomize(MAX_CYCLE $IP_RANDOMIZE_MAX_CYCLE)
    //-> Print
    -> host0;

    host0
    -> Unqueue
    //-> PrintStats
    -> AggregateCounter(COUNT_STOP $COUNT, PRINT_USERTIME true)
    -> Discard;

    Idle -> [1]host0;
    Idle -> [2]host0;
    Idle -> [3]host0;
    host0[1] -> Discard;
    host0[2] -> Discard;
    host0[3] -> Discard;
};

elementclass IP6PacketForward {
    host0 :: IP6Router4Port(192.168.0.1);

    //Script(write host0/rt.add 192.168.0.0/24 0);
    //Script(write host0/rt.add 0.0.0.0/0.0.0.0 1);
    Script(print_usertime, write host0/rt.generate $IP6_RT_SIZE 0, print_usertime);

    input[0]
    -> Clone($COUNT)
    -> Unqueue
    -> IP6Randomize(MAX_CYCLE $IP6_RANDOMIZE_MAX_CYCLE)
    //-> Print
    -> host0;

    host0
    -> Unqueue
    //-> PrintStats
    -> AggregateCounter(COUNT_STOP $COUNT, PRINT_USERTIME true)
    -> Discard;

    Idle -> [1]host0;
    Idle -> [2]host0;
    Idle -> [3]host0;
    host0[1] -> Discard;
    host0[2] -> Discard;
    host0[3] -> Discard;
};

elementclass XIAPacketForward {
    host0 :: Router4PortDummyCache(RE SELF_AD);

    Script(write host0/n/proc/rt_AD/rt.add - 5);
    Script(write host0/n/proc/rt_HID/rt.add - 5);
    Script(write host0/n/proc/rt_SID/rt.add - 5);
    Script(write host0/n/proc/rt_CID/rt.add - 5);

    Script(write host0/n/proc/rt_AD/rt.add SELF_AD 4);
    Script(print_usertime, write host0/n/proc/rt_AD/rt.generate AD $AD_RT_SIZE 0, print_usertime);

    Script(print_usertime, write host0/n/proc/rt_CID/rt.generate CID $CID_RT_SIZE 0, print_usertime);

    input[0]
    -> Clone($COUNT)
    -> Unqueue
    -> XIARandomize(XID_TYPE AD, MAX_CYCLE $AD_RANDOMIZE_MAX_CYCLE)
    //-> XIAPrint
    -> host0;

    input[1]
    -> Clone($COUNT)
    -> Unqueue
    -> XIARandomize(XID_TYPE CID, MAX_CYCLE $CID_RANDOMIZE_MAX_CYCLE)
    //-> XIAPrint
    -> host0;

    input[2]	// no clone
    -> XIARandomize(XID_TYPE AD, MAX_CYCLE $AD_RANDOMIZE_MAX_CYCLE)
    -> host0;

    host0 -> Unqueue
    //-> PrintStats
    -> AggregateCounter(COUNT_STOP $COUNT, PRINT_USERTIME true)
    -> Discard;

    Idle -> [1]host0;
    Idle -> [2]host0;
    Idle -> [3]host0;
    host0[1] -> Discard;
    host0[2] -> Discard;
    host0[3] -> Discard;
};
