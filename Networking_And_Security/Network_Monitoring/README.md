# Network Monitoring Lab 

## Description 

The purpose of this lab was to familiarize myself with Node Exporter (a program that exposes system metrics) and Prometheus (a program that scrapes and analyzes exposed metrics). The goal was to test Prometheus' effectiveness in detecting and alerting on Denial of Service (DoS) attacks.

The lab topology consists of a Raspberry Pi 4 (running Node Exporter) and a laptop (running Prometheus):

Raspberry Pi 4 (Node Exporter) → Laptop (Prometheus)

## Procedure 

#  - alert_rules.yml 

    -  This file contains all the rules for detection for example: 
    
      - High amounts of HTTP traffic 
      - Authentication failures 
      - High CPU usage 
      - High CPU core temp 
      - HTTP error codes 
      - High system load
      - High TCP retransmission 
      - High transmission rates
      - High UDP datagrams recieved 
      - High TCP SYN retransmissions

      Typically the metrics for evaluation are written as some value over 1 minutes, then transformed into that value per second then compared
      
#  - alertmanager.yml

    - This file will contain all alert managing applications, the ones I included: 
      - Slack Channel 
      - Email
    
#  - prometheus.yml 

    -  This file contains alertmanager config, alert_rules config, information about the target to scrape 


So what I did was try various amounts of Denial of Service Floods( layer 4 and 7): 
  - TCP SYN
  - UDP
  - TCP ACK
  - HTTP GET
  - HTTP POST

I measured key factors such as:

Latency between the attack and alert generation
Time delay between alert firing and alert delivery via email/Slack
This helped determine whether Prometheus was effective in detecting and alerting DoS attacks in a timely manner.

## Results and Conclusion 


After performing a **DoS attack on my Raspberry Pi 4**, I observed the following:  

- Alerts took anywhere from **30 to 90 seconds** to transition from a `"pending"` state to `"firing."`  
- Once firing, alerts appeared on **Slack within 30 to 90 seconds**, while **email alerts had an additional 2-minute delay**.  
- My **evaluation interval** was set to **10 seconds**, and my **scrape interval** was **15 seconds**.  

### **Inconsistent Behavior**  
I noticed **irregular alert behavior**, where:  
- Sometimes an alert would trigger correctly, but after **3–5 evaluations**, it would revert to `"pending"` or fail to register entirely.  
- This could be due to **misconfiguration** or **system limitations**.  

### **Latency and System Limitations**  
A key limitation I observed was **latency**. Large-scale DoS attacks could:  
- **Overwhelm Prometheus' ability to scrape data**, especially on a **lightweight system like the Raspberry Pi 4**.  
- Result in **missing or delayed alerts**, reducing effectiveness.  

### **Conclusion**  
While **Prometheus did detect DoS attacks**, the **latency issue** is a **significant concern**. In a real-world scenario:  
- **Rapid detection and prevention** are critical for **maintaining system availability**.  
- If integrated with an **Intrusion Prevention System (IPS)**, delays or inconsistent metrics could be **detrimental**.  

### **Final Thoughts**  
✅ **Pros**:  
✔️ Easy to set up  
✔️ Provides valuable system metrics  

⚠️ **Cons**:  
❌ Overhead and latency in metric collection  
❌ Requires a more powerful system for real-time DoS detection  

Would I recommend Prometheus for **real-time DoS detection**?  
**Possibly, but only with proper tuning and sufficient hardware resources.**  
