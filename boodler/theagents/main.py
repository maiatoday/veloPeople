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
cdtray = csounds.cdTray

computerhum = [
   csounds.tubeFan,
   csounds.insideComputer
]

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

personhum = psounds.apneu

class Example(agent.Agent):
    def run(self):
        self.post_listener_agent(ExampleGone(), hold=True)
        self.post_listener_agent(ExampleThere(), hold=True)
        self.post_listener_agent(ExampleStart(), hold=True)
        self.post_listener_agent(ExampleStop(), hold=True)

class ExampleGone(agent.Agent):
    selected_event = 'gone'
    def run(self):
        self.listen(hold=True)
    def receive(self, event):
        self.sched_note(csounds.insideComputer)

class ExampleThere(agent.Agent):
    selected_event = 'there'
    def run(self):
        self.listen(hold=True)
    def receive(self, event):
        self.sched_note(psounds.Sigh1)

class ExampleStart(agent.Agent):
    selected_event = 'start'
    def run(self):
        self.listen(hold=True)
    def receive(self, event):
        self.sched_note(csounds.Switch_06s)

class ExampleStop(agent.Agent):
    selected_event = 'stop'
    def run(self):
        self.listen(hold=True)
    def receive(self, event):
        self.sched_note(csounds.Switch_09s)

class BackgroundGone(agent.Agent):
    def run(self):
        p=random.uniform(0.2,0.5)
        v=random.uniform(0.7,1.3)
        d=random.uniform(0.8,1.0)
        dur=self.sched_note(computerhum, pitch=p, volume=v)
        self.resched(dur*d)

class SwitchSounds(agent.Agent):
    def run(self):
        ag = play.RepeatSoundShuffleList(5, 12, 2, switchsounds)
        ag2 = manage.VolumeModulateAgent(ag, 0.7)
        self.sched_agent(ag2)

class AllGone(agent.Agent):
   def run(self):
      self.c_soundsChan = self.new_channel(0)
      ag = BackgroundGone()
      self.sched_agent(ag, chan=self.c_soundsChan)
      ag = SwitchSounds()
      self.sched_agent(ag, chan=self.c_soundsChan)

class AllThere(agent.Agent):
   def run(self):
      self.p_soundsChan = self.new_channel(0)
      ag = BackgroundGone()
      self.sched_agent(ag, chan=self.p_soundsChan)
      ag = SwitchSounds()
      self.sched_agent(ag, chan=self.p_soundsChan)
