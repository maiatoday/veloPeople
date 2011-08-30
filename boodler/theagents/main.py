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
    psounds.ClearingThroat1,
    psounds.ClearingThroat2,
    psounds.ClearingThroat3,
    psounds.ClearingThroat_Female,
    psounds.clear_throat,
    psounds.ManClearingThroat
]

sighsniffyawn = [
        psounds.Sigh1,
        psounds.Sigh3,
        psounds.anotheryawn,
        psounds.breatheInMale,
        psounds.sniffle2,
        psounds.yawn,
        psounds.yawning1
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
        self.p_chan = self.new_channel(0)
        ag = AllThere()
        self.sched_agent(ag, chan=self.p_chan)
        self.listen(hold=True)
    def receive(self, event, val = "there"):
        if val == "gone":
            print "no-one there, all gone"
            self.c_chan.set_volume(1,5)
            self.p_chan.set_volume(0,1)
        else:
            print "someone there"
            self.c_chan.set_volume(0,5)
            self.p_chan.set_volume(1,1)

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
        ag = play.IntermittentSoundsList(1, 7, 1, 1, 0.5, 1.0, 0, switchsounds)
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
        ag = play.IntermittentSoundsList(5, 15, 1, 1, 0.5, 1.0, 0, sighsniffyawn)
        #ag = play.SoundShuffleMix(sighsniffyawn)
        ag2 = manage.VolumeModulateAgent(ag, 0.7)
        self.sched_agent(ag2)
   
class ClearThroat(agent.Agent):
    def run(self):
        ag = play.IntermittentSoundsList(2, 7, 1, 1, 0.5, 1.0, 0, clearthroat)
        #ag = play.SoundShuffleMix(sighsniffyawn)
        ag2 = manage.VolumeModulateAgent(ag, 0.7)
        self.sched_agent(ag2)

class AllThere(agent.Agent):
   def run(self):
      ag = SighSniffYawn()
      self.sched_agent(ag)
      ag = ClearThroat()
      self.sched_agent(ag)
