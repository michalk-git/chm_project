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
    
    # expect("===RTC===> St-Init  Obj=AO_Member<0>,State=QP::QHsm::top->NULL")
    # expect("===RTC===> St-Init  Obj=AO_Member<1>,State=QP::QHsm::top->NULL")
    # expect("===RTC===> St-Init  Obj=AO_Member<2>,State=QP::QHsm::top->NULL")
    # expect("===RTC===> St-Init  Obj=AO_Member<3>,State=QP::QHsm::top->NULL")
    # expect("===RTC===> St-Init  Obj=AO_Member<4>,State=QP::QHsm::top->NULL")
    

    #expect("===RTC===> St-Init  Obj=AO_Member<0>,State=QP::QHsm::top->NULL")
    current_obj(OBJ_SM_AO, "AO_HealthMonitor")
    current_obj(OBJ_TE, "timeEvt_kick")
    #glb_filter(GRP_AO, GRP_TE)
    

   

    
def on_setup():
    print("on_setup")

def on_teardown():
    print("on_teardown")

# tests...
test("gfg")
tick()
expect("0000000001 TE0-Post Obj=timeEvt_kick,Sig=TIMER_KICK_SIG,AO=AO_HealthMonitor")
expect("0000000002 AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=TIMER_KICK_SIG,*")
expect("0000000003 AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=TIMER_KICK_SIG,*")
expect("0000000004 Trg-Done QS_RX_TICK")

#Test 1:
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
test("Post TIMER_UPDATE_SIG (no subscribers)")
post("TIMER_UPDATE_SIG")
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=TIMER_UPDATE_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=TIMER_UPDATE_SIG,*")
expect("@timestamp Trg-Done QS_RX_EVENT")
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 2:
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
test("Post ALIVE_SIG (no subscribers)")
post("ALIVE_SIG", pack("<B", 2))
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=ALIVE_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=ALIVE_SIG,*")
expect("@timestamp Trg-Done QS_RX_EVENT")
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 3:
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
test("Post SUBSCRIBE_SIG to subscribe user to AO_Member 1")
post("SUBSCRIBE_SIG", pack("<BB", 1,0))
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
expect("@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=SUBSCRIBE_ACKNOWLEDGE_SIG,Pool=1,Ref=1>,*")
expect("@timestamp Trg-Done QS_RX_EVENT")
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 4:
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
test("Post UNSUBSCRIBE_SIG to unsubscribe non-existant user (no subscribers)")
post("UNSUBSCRIBE_SIG", pack("<BB",1,0))
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,*")
expect("@timestamp Trg-Done QS_RX_EVENT")
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 5:
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
test("Post TIMER_KICK_SIG (no subscribers)")
post("TIMER_KICK_SIG")
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=TIMER_KICK_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=TIMER_KICK_SIG,*")
expect("@timestamp Trg-Done QS_RX_EVENT")
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
#Test 6:
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
test("Post SUBSCRIBE_SIG to subscribe user with id=0 to AO_Member 1")
post("SUBSCRIBE_SIG", pack("<BB", 1,0))
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
expect("@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=SUBSCRIBE_ACKNOWLEDGE_SIG,Pool=1,Ref=1>,*")
expect("@timestamp Trg-Done QS_RX_EVENT")


test("Post TIMER_UPDATE_SIG (one subscriber)", NORESET)
post("TIMER_UPDATE_SIG")
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=TIMER_UPDATE_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=TIMER_UPDATE_SIG,*")
expect("@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<4>,Evt<Sig=REQUEST_UPDATE_SIG,Pool=1,Ref=2>,Que<Free=0,Min=255>")
expect("@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<3>,Evt<Sig=REQUEST_UPDATE_SIG,Pool=1,Ref=2>,Que<Free=0,Min=255>")
expect("@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<2>,Evt<Sig=REQUEST_UPDATE_SIG,Pool=1,Ref=2>,Que<Free=0,Min=255>")
expect("@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=REQUEST_UPDATE_SIG,Pool=1,Ref=2>,Que<Free=0,Min=255>")
expect("@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<0>,Evt<Sig=REQUEST_UPDATE_SIG,Pool=1,Ref=2>,Que<Free=0,Min=255>")
expect("@timestamp Trg-Done QS_RX_EVENT")

test("Post ALIVE_SIG (one subscriber)", NORESET)
post("ALIVE_SIG")
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=ALIVE_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=ALIVE_SIG,*")
expect("@timestamp Trg-Done QS_RX_EVENT")

test("Post UNSUBSCRIBE_SIG in order to unsubscribe user", NORESET)
post("UNSUBSCRIBE_SIG", pack("<BB", 1,0))
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,*")
expect("@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=UNSUBSCRIBE_ACKNOWLEDGE_SIG,*")
expect("@timestamp Trg-Done QS_RX_EVENT")
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#