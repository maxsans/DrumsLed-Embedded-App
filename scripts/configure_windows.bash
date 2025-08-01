
# Allow incoming UDP packets on port 9999
netsh advfirewall firewall add rule name="UDP 9999" dir=in action=allow protocol=UDP localport=9999
# Allow outgoing UDP packets on port 9999
netsh advfirewall firewall add rule name="UDP 9999" dir=out action=allow protocol=UDP localport=9999

# Allow incoming TCP packets on port 8888
netsh advfirewall firewall add rule name="TCP 8888" dir=in action=allow protocol=TCP localport=8888
# Allow outgoing TCP packets on port 8888
netsh advfirewall firewall add rule name="TCP 8888" dir=out action=allow protocol=TCP localport=8888
