#!/usr/bin/python3
from mininet.topo import Topo
from mininet.net import Mininet
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel
from mininet.cli import CLI
from mininet.node import RemoteController
class FinalTopo(Topo):
    def build(self):
        sw1 = self.addSwitch('sw1')
        sw2 = self.addSwitch('sw2')
        sw3 = self.addSwitch('sw3')
        sw4 = self.addSwitch('sw4')
        sw5 = self.addSwitch('sw5')
        sw6 = self.addSwitch('sw6')
        h101 = self.addHost('h101', mac='00:00:00:00:00:01', ip='128.114.1.101/24',
        defaultRoute="h101-eth0")
        h102 = self.addHost('h102', mac='00:00:00:00:00:02', ip='128.114.1.102/24',
        defaultRoute="h102-eth0")
        h103 = self.addHost('h103', mac='00:00:00:00:00:03', ip='128.114.1.103/24',
        defaultRoute="h103-eth0")
        h104 = self.addHost('h104', mac='00:00:00:00:00:04', ip='128.114.1.104/24',
        defaultRoute="h104-eth0")
        h201 = self.addHost('h201', mac='00:00:00:00:00:05', ip='128.114.2.201/24',
        defaultRoute="h201-eth0")
        h202 = self.addHost('h202', mac='00:00:00:00:00:06', ip='128.114.2.202/24',
        defaultRoute="h202-eth0")
        h203 = self.addHost('h203', mac='00:00:00:00:00:07', ip='128.114.2.203/24',
        defaultRoute="h203-eth0")
        h204 = self.addHost('h204', mac='00:00:00:00:00:08', ip='128.114.2.204/24',
        defaultRoute="h204-eth0")
        h_trust = self.addHost('h_trust', mac='00:00:00:00:00:09',
        ip='192.47.38.109/24', defaultRoute="h_trust-eth0")
        h_untrust = self.addHost('h_untrust', mac='00:00:00:00:00:10',
        ip='108.35.24.113/24', defaultRoute="h_untrust-eth0")
        h_server = self.addHost('h_server', mac='00:00:00:00:00:11',
        ip='128.114.3.178/24', defaultRoute="h_server-eth0")
        print("Hosts and switches created. Adding links...")
        self.addLink(h_trust, sw5, port1=0, port2=1)
        self.addLink(h_untrust, sw5, port1=0, port2=2)
        self.addLink(sw5, sw6, port1=3, port2=3)
        self.addLink(sw6, h_server, port1=4, port2=0)
        self.addLink(h101, sw1, port1=0, port2=5)
        self.addLink(h102, sw1, port1=0, port2=6)
        self.addLink(h103, sw2, port1=0, port2=7)
        self.addLink(h104, sw2, port1=0, port2=8)
        self.addLink(h201, sw3, port1=0, port2=9)
        self.addLink(h202, sw3, port1=0, port2=10)
        self.addLink(h203, sw4, port1=0, port2=11)
        self.addLink(h204, sw4, port1=0, port2=12)
        self.addLink(sw3, sw5, port1=5, port2=5)
        self.addLink(sw4, sw5, port1=6, port2=6)
        self.addLink(sw2, sw5, port1=9, port2=9)
        self.addLink(sw1, sw5, port1=10, port2=10)
        print("Links added successfully.")
    def configure():
        topo = FinalTopo()
        controller = RemoteController('c0', ip='127.0.0.1', port=6633)
        net = Mininet(topo=topo, controller=controller)
        net.start()
        try:
        CLI(net)
        finally:
        net.stop()
    if __name__ == '__main__':
        setLogLevel('info')
        configure()