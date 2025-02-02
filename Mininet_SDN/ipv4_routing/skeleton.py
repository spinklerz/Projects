from pox.core import core
import pox.openflow.libopenflow_01 as of
log = core.getLogger()
class Final(object):
    def __init__(self, connection):
        self.connection = connection
        connection.addListeners(self)
    def do_final(self, packet, packet_in, port_on_switch, switch_id):
        DepartmentA = ['128.114.1.101', '128.114.1.102', '128.114.1.103',
        '128.114.1.104']
        DepartmentB = ['128.114.2.201', '128.114.2.202', '128.114.2.203',
        '128.114.2.204']
        BothDepartments = DepartmentA + DepartmentB
        print(packet)
        # ARP handler

        msg_arp = of.ofp_flow_mod()
        msg_arp.priority = 9
        msg_arp.match.dl_type = 0x0806 # ARP
        msg_arp.actions.append(of.ofp_action_output(port=of.OFPP_FLOOD))
        self.connection.send(msg_arp)

        # TCP rule (flood)
        match_tcp = of.ofp_match(dl_type=0x0800, nw_proto=6) # IPv4 + TCP
        msg_tcp = of.ofp_flow_mod()
        msg_tcp.priority = 99
        msg_tcp.match = match_tcp
        msg_tcp.actions.append(of.ofp_action_output(port=of.OFPP_FLOOD))
        self.connection.send(msg_tcp)

        # Trusted host restrictions for ICMP to Department B
        trusted_host = '192.47.38.109'
        for ip in DepartmentB:
        msg_icmp_trusted = of.ofp_flow_mod()
        msg_icmp_trusted.priority = 90
        msg_icmp_trusted.match = of.ofp_match(dl_type=0x0800, nw_proto=1,
        nw_src=trusted_host, nw_dst=ip)
        self.connection.send(msg_icmp_trusted)

        # Untrusted host restrictions for ICMP to both departments
        untrusted_host = '108.35.24.113'
        for ip in BothDepartments:
        msg_icmp_untrusted = of.ofp_flow_mod()
        msg_icmp_untrusted.priority = 98
        msg_icmp_untrusted.match = of.ofp_match(dl_type=0x0800, nw_proto=1,
        nw_src=untrusted_host, nw_dst=ip)
        self.connection.send(msg_icmp_untrusted)

        # Department A to Department B ICMP
        for ipA in DepartmentA:
        for ipB in DepartmentB:
        msg_icmp_dep = of.ofp_flow_mod()
        msg_icmp_dep.priority = 102
        msg_icmp_dep.match = of.ofp_match(dl_type=0x0800, nw_proto=1,
        nw_src=ipA, nw_dst=ipB)
        self.connection.send(msg_icmp_dep)

        # Trusted host cannot send ICMP to LLM_Server
        llm_server = '128.114.3.178'
        msg_icmp_trusted_llm = of.ofp_flow_mod()
        msg_icmp_trusted_llm.priority = 5000
        msg_icmp_trusted_llm.match = of.ofp_match(dl_type=0x0800, nw_proto=1,
        nw_src='192.47.38.109', nw_dst=llm_server)
        self.connection.send(msg_icmp_trusted_llm)

        # Untrusted host cannot send ICMP to LLM_Server
        msg_icmp_untrusted_llm = of.ofp_flow_mod()
        msg_icmp_untrusted_llm.priority = 101
        msg_icmp_untrusted_llm.match = of.ofp_match(dl_type=0x0800, nw_proto=1,
        nw_src=untrusted_host, nw_dst=llm_server)
        self.connection.send(msg_icmp_untrusted_llm)
        # Flood everything else

        msg_flood = of.ofp_flow_mod()
        msg_flood.priority = 1
        msg_flood.match = of.ofp_match()
        msg_flood.actions.append(of.ofp_action_output(port=of.OFPP_FLOOD))
        self.connection.send(msg_flood)
        
    def _handle_PacketIn(self, event):
        packet = event.parsed
        if not packet.parsed:
        log.warning("Ignoring incomplete packet")
        return
        packet_in = event.ofp
        self.do_final(packet, packet_in, event.port, event.dpid)
    def launch():
        def start_switch(event):
        log.debug("Controlling %s" % (event.connection,))
        Final(event.connection)
        core.openflow.addListenerByName("ConnectionUp", start_switch)
