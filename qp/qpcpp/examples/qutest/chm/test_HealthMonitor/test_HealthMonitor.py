# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# preamble...
def on_reset():
    expect_pause()
    continue_test()
    glb_filter(GRP_AO, GRP_TE)
    current_obj(OBJ_SM_AO, "AO_HealthMonitor")
    current_obj(OBJ_TE, "l_HealthMonitor.timeEvt_request_update")
   

    
def on_setup():
    print("on_setup")

def on_teardown():
    print("on_teardown")

# tests...




#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
test("Post TIMER_UPDATE_SIG (no subscribers)")
post("TIMER_UPDATE_SIG")
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=TIMER_UPDATE_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=TIMER_UPDATE_SIG,*")
expect("@timestamp Trg-Done QS_RX_EVENT")
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
test("Post ALIVE_SIG (no subscribers)")
post("ALIVE_SIG", pack("<B", 2))
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=ALIVE_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=ALIVE_SIG,*")
expect("@timestamp Trg-Done QS_RX_EVENT")
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
test("Post SUBSCRIBE_SIG to subscribe user to AO_Member 1")
post("SUBSCRIBE_SIG", pack("<BB", 1,0))
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
expect("@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=SUBSCRIBE_ACKNOWLEDGE_SIG,Pool=1,Ref=1>,*")
expect("@timestamp Trg-Done QS_RX_EVENT")
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
test("Post UNSUBSCRIBE_SIG to unsubscribe non-existant user (no subscribers)")
post("UNSUBSCRIBE_SIG", pack("<BB",1,0))
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,*")
expect("@timestamp Trg-Done QS_RX_EVENT")
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
test("Post TIMER_KICK_SIG (no subscribers)")
post("TIMER_KICK_SIG")
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=TIMER_KICK_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=TIMER_KICK_SIG,*")
expect("@timestamp Trg-Done QS_RX_EVENT")
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
test("Post SUBSCRIBE_SIG to subscribe user with id=0 to AO_Member 1")
post("SUBSCRIBE_SIG", pack("<BB", 1,0))
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=SUBSCRIBE_SIG,*")
expect("@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=SUBSCRIBE_ACKNOWLEDGE_SIG,Pool=1,Ref=1>,*")
expect("@timestamp Trg-Done QS_RX_EVENT")

test("Post ALIVE_SIG (one subscriber)", NORESET)
post("ALIVE_SIG")
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=ALIVE_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=ALIVE_SIG,*")
expect("@timestamp Trg-Done QS_RX_EVENT")

test("Post TIMER_UPDATE_SIG (one subscriber)", NORESET)
post("TIMER_UPDATE_SIG")
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=TIMER_UPDATE_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=TIMER_UPDATE_SIG,*")
expect("@timestamp Trg-Done QS_RX_EVENT")

test("Post UNSUBSCRIBE_SIG in order to unsubscribe user", NORESET)
post("UNSUBSCRIBE_SIG", pack("<BB", 1,0))
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,*")
expect("@timestamp AO-GetL  Obj=AO_HealthMonitor,Evt<Sig=UNSUBSCRIBE_SIG,*")
expect("@timestamp AO-Post  Sdr=AO_HealthMonitor,Obj=AO_Member<1>,Evt<Sig=UNSUBSCRIBE_ACKNOWLEDGE_SIG,*")
expect("@timestamp Trg-Done QS_RX_EVENT")
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#