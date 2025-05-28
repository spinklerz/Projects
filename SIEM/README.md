# SIEM Development

## Resources used

- MacOs laptop ( Dashboard/Management )
- Raspberry pi 4 ( Log Collector/Forwarder )
- Windows 11 machine ( Endpoint )
- Ubuntu VM ( Log Processing/Rule enforcement/etc.... )

## Example Workflow

1.) Windows(Endpoint) -> Raspberry pi 4(clean/store/collect)<br>
2.) Raspberry pi 4(clean/store/collect) -> Ubuntu(process/alert)<br>
3.) Ubuntu(process/alert) -> macOs( Visualize/Manage)
<br>

## Tech Stack(Draft)(May 28th 2025 update)

- MacOs
  -
- Windows
  - ElasticSearch
  - Kibana
  - Winlogbeat
- Raspberry pi 4
  - vsyslog
