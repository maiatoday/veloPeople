from boopak.package import *
from boodle import agent
import random

play = bimport('org.boodler.play')
manage = bimport('org.boodler.manage')

csounds = bimport('name.maiatoday.csounds')
psounds = bimport('name.maiatoday.psounds')


switchsounds = [
        csounds.Switch_06s,
        csounds.Switch_09s,
        csounds.Switch_16s,
        csounds.Switch_23s,
        csounds.Switch_28s,
        csounds.Switch_35s,
        csounds.Switch_36s
]

beepsound = csounds.cleanModemBeep

clearthroat = [
    psounds.clear_1_01,
    psounds.clear_2_01,
    psounds.clear_4_01,
    psounds.clear_4_02,
    psounds.clear_4_03,
    psounds.clear_5_01,
    psounds.clear_5_02,
    psounds.clear_5_03,
    psounds.clear_5_04,
    psounds.ClearingThroat1,
    psounds.ClearingThroat2,
    psounds.ClearingThroat3,
    psounds.ClearingThroat_Female,
    psounds.clear_throat,
    psounds.cough_1_01,
    psounds.cough_2_01,
    psounds.cough_5_01,
    psounds.cough_5_02,
    psounds.ManClearingThroat
]

sighsniffyawn = [
    psounds.anotheryawn,
    psounds.breatheInMale,
    psounds.sigh_1_01,
    psounds.Sigh1,
    psounds.Sigh3,
    psounds.sigh_4_01,
    psounds.sigh_5_01,
    psounds.sigh_5_02,
    psounds.sigh_5_03,
    psounds.sigh_5_04,
    psounds.sigh_5_05,
    psounds.sight_1_02,
    psounds.sniff_1_01,
    psounds.sniff_2_01,
    psounds.sniff_2_02,
    psounds.sniffle2,
    psounds.yawn_1_01,
    psounds.yawn_2_01,
    psounds.yawn_3_01,
    psounds.yawning1,
    psounds.yawn
]

class LumoSoundListeners(agent.Agent):
    def run(self):
        ag = ListenGone()
        self.sched_agent(ag)
        self.post_listener_agent(ListenThere(), hold=True)
        self.post_listener_agent(ListenStart(), hold=True)
        self.post_listener_agent(ListenStop(), hold=True)

class ListenGone(agent.Agent):
    selected_event = 'someone'
    def run(self):
        self.c_chan = self.new_channel()
        ag = AllGone()
        self.sched_agent(ag, chan=self.c_chan)
        self.p_chan_sniff = self.new_channel(0)
        ag = SighSniffYawn()
        self.sched_agent(ag, chan=self.p_chan_sniff)
        self.p_chan_hrmm = self.new_channel(0)
        ag = ClearThroat()
        self.sched_agent(ag, chan=self.p_chan_hrmm)
        self.listen(hold=True)
    def receive(self, event, val = "there"):
        if val == "gone":
            print "no-one there, all gone"
            self.c_chan.set_volume(1,5)
            self.p_chan_sniff.set_volume(0,1)
            self.p_chan_hrmm.set_volume(0,1)
        elif val == "start":
            self.c_chan.set_volume(1,5)
            self.p_chan_sniff.set_volume(0,1)
            self.p_chan_hrmm.set_volume(0,1)
        elif val == "stop":
            self.c_chan.set_volume(0,1)
            self.p_chan_sniff.set_volume(0,1)
            self.p_chan_hrmm.set_volume(0,1)
        else:
            print "someone there"
            self.c_chan.set_volume(0,5)
            if val == "theresniff": 
                self.p_chan_sniff.set_volume(1,1)
                self.p_chan_hrmm.set_volume(0,1)
            else:
                self.p_chan_sniff.set_volume(1,1)
                self.p_chan_hrmm.set_volume(1,1)

class ListenThere(agent.Agent):
    selected_event = 'there'
    def run(self):
        self.listen(hold=True)
    def receive(self, event, val = "1"):
        val = int(val) - 1
        samp = sighsniffyawn[val]
        self.sched_note(samp)

class ListenStart(agent.Agent):
    selected_event = 'start'
    def run(self):
        self.listen(hold=True)
    def receive(self, event):
        self.sched_note(csounds.Switch_06s)

class ListenStop(agent.Agent):
    selected_event = 'stop'
    def run(self):
        self.listen(hold=True)
    def receive(self, event):
        self.sched_note(csounds.Switch_09s)

class BackgroundGone(agent.Agent):
    def run(self):
        #p=random.uniform(0.2,0.5)
        p=1.0
        v=random.uniform(0.7,1.3)
        d=random.uniform(1.0,10.0)
        dur=self.sched_note(beepsound, pitch=p, volume=v)
        self.resched(dur*d)

class SwitchSounds(agent.Agent):
    def run(self):
        #ag = play.RepeatSoundShuffleList(1, 3, 5, switchsounds)
        ag = play.IntermittentSoundsList(1, 7, 1, 1, 0.5, 1.0, 1, switchsounds)
        ag2 = manage.VolumeModulateAgent(ag, 0.7)
        self.sched_agent(ag2)

class AllGone(agent.Agent):
   def run(self):
      ag = BackgroundGone()
      self.sched_agent(ag)
      ag = SwitchSounds()
      self.sched_agent(ag)

class SighSniffYawn(agent.Agent):
    def run(self):
        ag = play.IntermittentSoundsList(1, 9, 1, 1, 0.5, 1.0, 1, sighsniffyawn)
        #ag = play.SoundShuffleMix(sighsniffyawn)
        ag2 = manage.VolumeModulateAgent(ag, 0.7)
        self.sched_agent(ag2)
   
class ClearThroat(agent.Agent):
    def run(self):
        ag = play.IntermittentSoundsList(2, 7, 1, 1, 0.5, 1.0, 1, clearthroat)
        #ag = play.SoundShuffleMix(sighsniffyawn)
        ag2 = manage.VolumeModulateAgent(ag, 0.7)
        self.sched_agent(ag2)

class AllThere(agent.Agent):
   def run(self):
      ag = SighSniffYawn()
      self.sched_agent(ag)
      ag = ClearThroat()
      self.sched_agent(ag)
