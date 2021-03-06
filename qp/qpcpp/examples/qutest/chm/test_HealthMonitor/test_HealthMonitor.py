# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_reset():
    expect_pause()
    glb_filter(GRP_AO, GRP_TE)
    
   
    continue_test()
    expect("@timestamp AO-Subsc Obj=AO_Member<0>,Sig=REQUEST_UPDATE_SIG")
    expect("@timestamp AO-Subsc Obj=AO_Member<1>,Sig=REQUEST_UPDATE_SIG")
    expect("@timestamp AO-Subsc Obj=AO_Member<2>,Sig=REQUEST_UPDATE_SIG")
    expect("@timestamp AO-Subsc Obj=AO_Member<3>,Sig=REQUEST_UPDATE_SIG")
    expect("@timestamp AO-Subsc Obj=AO_Member<4>,Sig=REQUEST_UPDATE_SIG")
    expect("@timestamp TE0-Arm  Obj=timeEvt_kick,AO=AO_HealthMonitor,Tim=*,Int=*")
    expect("@timestamp TE0-Arm  Obj=timeEvt_request_update,AO=AO_HealthMonitor,Tim=*,Int=*")
    expect("@timestamp TE0-Arm  Obj=timeEvt_tick,AO=AO_HealthMonitor,Tim=*,Int=*")
    current_obj(OBJ_SM_AO, "AO_HealthMonitor")

    

   

    
def on_setup():
    print("on_setup")

def on_teardown():
    print("on_teardown")


def TestTimerUpdateNoSubs():
    test("Post TIMER_UPDATE_SIG (no subscribers)")
    post("TIMER_UPDATE_SIG")
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=TIMER_UPDATE_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=TIMER_UPDATE_SIG,*")
    expect("@timestamp Trg-Done QS_RX_EVENT")


def SubscribeSameUserTwice():
    test("Post SUBSCRIBE_SIG to subscribe user to AO_Member 1")
    post("SUBSCRIBE_SIG", pack("<BB", 1, 0))
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
    expect("@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=SUBSCRIBE_ACKNOWLEDGE_SIG,*")
    expect("@timestamp Trg-Done QS_RX_EVENT")
    test("Post SUBSCRIBE_SIG to subscribe same user twice", NORESET)
    post("SUBSCRIBE_SIG", pack("<BB", 1, 0))
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
    expect("@timestamp Trg-Done QS_RX_EVENT")



def UnsubscribeTwice():
    test("Post SUBSCRIBE_SIG to subscribe user to AO_Member 1")
    post("SUBSCRIBE_SIG", pack("<BB", 1, 0))
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
    expect(
        "@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=SUBSCRIBE_ACKNOWLEDGE_SIG,Pool=1,Ref=1>,*")
    expect("@timestamp Trg-Done QS_RX_EVENT")
    test("Post UNSUBSCRIBE_SIG to unsubscribe user", NORESET)
    post("UNSUBSCRIBE_SIG", pack("<BB", 1, 0))
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,Pool=1,Ref=1>,Que<Free=5,Min=5>")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,Pool=1,Ref=1>")
    expect(
        "@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=UNSUBSCRIBE_ACKNOWLEDGE_SIG,Pool=1,Ref=1>,Que<Free=0,Min=255>")
    expect("@timestamp Trg-Done QS_RX_EVENT")
    test("Post UNSUBSCRIBE_SIG to unsubscribe user twice", NORESET)
    post("UNSUBSCRIBE_SIG", pack("<BB", 1, 0))
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,*")
    expect("@timestamp Trg-Done QS_RX_EVENT")

def SubscribeUserAfterUnsubs():
    test("Post SUBSCRIBE_SIG to subscribe user to AO_Member 1")
    post("SUBSCRIBE_SIG", pack("<BB", 1, 0))
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
    expect(
        "@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=SUBSCRIBE_ACKNOWLEDGE_SIG,Pool=1,Ref=1>,*")
    expect("@timestamp Trg-Done QS_RX_EVENT")
    test("Post UNSUBSCRIBE_SIG to unsubscribe user", NORESET)
    post("UNSUBSCRIBE_SIG", pack("<BB", 1, 0))
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,Pool=1,Ref=1>,Que<Free=5,Min=5>")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,Pool=1,Ref=1>")
    expect(
        "@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=UNSUBSCRIBE_ACKNOWLEDGE_SIG,Pool=1,Ref=1>,Que<Free=0,Min=255>")
    expect("@timestamp Trg-Done QS_RX_EVENT")
    test("Post SUBSCRIBE_SIG to subscribe user (with same id as the unsubscribed user)  to AO_Member 1", NORESET)
    post("SUBSCRIBE_SIG", pack("<BB", 1, 0))
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
    expect(
        "@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=SUBSCRIBE_ACKNOWLEDGE_SIG,Pool=1,Ref=1>,*")
    expect("@timestamp Trg-Done QS_RX_EVENT")

def SubscribeSameUserDiffMembers():
     test("Post SUBSCRIBE_SIG to subscribe user to AO_Member 1")
     post("SUBSCRIBE_SIG", pack("<BB", 1, 0))
     expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
     expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
     expect("@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=SUBSCRIBE_ACKNOWLEDGE_SIG,Pool=1,Ref=1>,*")
     expect("@timestamp Trg-Done QS_RX_EVENT")
     test("Post SUBSCRIBE_SIG to subscribe same user to different AO_Member", NORESET)
     post("SUBSCRIBE_SIG", pack("<BB", 2, 0))
     expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
     expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
     expect("@timestamp Trg-Done QS_RX_EVENT")


def SubscribeUser():
    test("Post SUBSCRIBE_SIG to subscribe user to AO_Member 1")
    post("SUBSCRIBE_SIG", pack("<BB", 1, 0))
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
    expect("@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=SUBSCRIBE_ACKNOWLEDGE_SIG,Pool=1,Ref=1>,*")
    expect("@timestamp Trg-Done QS_RX_EVENT")


def UnsubscribeNonExistantUser():
    test("Post UNSUBSCRIBE_SIG to unsubscribe non-existant user (no subscribers)")
    post("UNSUBSCRIBE_SIG", pack("<BB", 1, 0))
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,*")
    expect("@timestamp Trg-Done QS_RX_EVENT")


def PostKickNoSubsc():
    test("Post TIMER_KICK_SIG (no subscribers)")
    post("TIMER_KICK_SIG")
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=TIMER_KICK_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=TIMER_KICK_SIG,*")
    expect("@timestamp Trg-Done QS_RX_EVENT")

def SubscAndUnsubsc():
    test("Post SUBSCRIBE_SIG to subscribe user with id=0 to AO_Member 1")
    post("SUBSCRIBE_SIG", pack("<BB", 1, 0))
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
    expect(
        "@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=SUBSCRIBE_ACKNOWLEDGE_SIG,Pool=1,Ref=1>,*")
    expect("@timestamp Trg-Done QS_RX_EVENT")

    test("Post UNSUBSCRIBE_SIG in order to unsubscribe user", NORESET)
    post("UNSUBSCRIBE_SIG", pack("<BB", 1, 0))
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,*")
    expect("@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=UNSUBSCRIBE_ACKNOWLEDGE_SIG,*")
    expect("@timestamp Trg-Done QS_RX_EVENT")

def SubscAndUpdate():
    test("Post SUBSCRIBE_SIG to subscribe user with id=0 to AO_Member 1")
    post("SUBSCRIBE_SIG", pack("<BB", 1, 0))
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
    expect(
        "@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=SUBSCRIBE_ACKNOWLEDGE_SIG,Pool=1,Ref=1>,*")
    expect("@timestamp Trg-Done QS_RX_EVENT")

    test("Post TIMER_UPDATE_SIG (one subscriber)", NORESET)
    post("TIMER_UPDATE_SIG")
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=TIMER_UPDATE_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=TIMER_UPDATE_SIG,*")
    expect(
        "@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<4>,Evt<Sig=REQUEST_UPDATE_SIG,Pool=1,Ref=2>,Que<Free=0,Min=255>")
    expect(
        "@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<3>,Evt<Sig=REQUEST_UPDATE_SIG,Pool=1,Ref=2>,Que<Free=0,Min=255>")
    expect(
        "@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<2>,Evt<Sig=REQUEST_UPDATE_SIG,Pool=1,Ref=2>,Que<Free=0,Min=255>")
    expect(
        "@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=REQUEST_UPDATE_SIG,Pool=1,Ref=2>,Que<Free=0,Min=255>")
    expect(
        "@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<0>,Evt<Sig=REQUEST_UPDATE_SIG,Pool=1,Ref=2>,Que<Free=0,Min=255>")
    expect("@timestamp Trg-Done QS_RX_EVENT")


def PostAliveIndexOutOfBounds():
    test("Post ALIVE_SIG with index which is out of bounds")
    post("ALIVE_SIG", pack("<B", 5))
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=ALIVE_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=ALIVE_SIG,*")
    expect("@timestamp Trg-Done QS_RX_EVENT")

def CallKickTimeEvt():
    test("CallKickTimeEvt")
    current_obj(OBJ_TE, "timeEvt_kick")
    tick()
    expect("@timestamp TE0-Post Obj=timeEvt_kick,Sig=TIMER_KICK_SIG,AO=AO_HealthMonitor")
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=TIMER_KICK_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=TIMER_KICK_SIG,*")
    expect("@timestamp Trg-Done QS_RX_TICK")



def CallRequestUpdateTimeEvt():
    test("CallRequestUpdateTimeEvt")
    current_obj(OBJ_TE, "timeEvt_request_update")
    tick()
    expect("@timestamp TE0-Post Obj=timeEvt_request_update,Sig=TIMER_UPDATE_SIG,AO=AO_HealthMonitor")
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=TIMER_UPDATE_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=TIMER_UPDATE_SIG,*")
    expect("@timestamp Trg-Done QS_RX_TICK")


def PostAliveNoSubsc():
    test("Post ALIVE_SIG (no subscribers)")
    post("ALIVE_SIG")
    expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=ALIVE_SIG,*")
    expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=ALIVE_SIG,*")
    expect("@timestamp Trg-Done QS_RX_EVENT")


#Test 1: Tests posting of TIMER_UPDATE_SIG signal when there are no subscribers
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
TestTimerUpdateNoSubs()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 2: Tests posting of ALIVE_SIG signal when there are no subscribers
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
PostAliveNoSubsc()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 3: Tests the case of one subscriber to the system
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
SubscribeUser()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 4: Check that unsubscribing non-existent user fails
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
UnsubscribeNonExistantUser()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 5: Tests posting of TIMER_KICK_SIG signal when there are no subscribers
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
PostKickNoSubsc()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 6: Tests subscribing and then unsubscribing
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
SubscAndUnsubsc()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 7: Tests subscribing and then posting TIMER_UPDATE_SIG signal
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
SubscAndUpdate()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 8: Tests sending ALIVE_SIG with index out of bounds
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
PostAliveIndexOutOfBounds()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 8: Tests trying to subscribe same user twice
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
SubscribeSameUserTwice()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 9: Tests trying to unsubscribe user twice
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
UnsubscribeTwice()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 10: Tests trying to subscribe same user after unsubscribing
 #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
SubscribeUserAfterUnsubs()
 #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 11: Tests trying to subscribe same user on different Member aos
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
SubscribeSameUserDiffMembers()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 12: Tests calling kick time event
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
CallKickTimeEvt()
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 13: Tests calling request update time event
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
CallRequestUpdateTimeEvt()

