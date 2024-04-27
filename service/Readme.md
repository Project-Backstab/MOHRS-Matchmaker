# Service

If you want to install this project as a service we have here instructions how to setup.

You can check systemd version like this:
```
systemd --version
```

We reload systemd
```
sudo systemctl daemon-reload
```

Copy the service file and modify the following values in mohrs.service: "ExecStart" and "WorkingDirectory" 
```
sudo cp service/mohrs.service /etc/systemd/system/
sudo nano /etc/systemd/system/mohrs.service
```

Enable and Start service:
```
sudo systemctl enable mohrs.service
sudo systemctl start mohrs.service
```

## Usage

```
sudo systemctl start mohrs
sudo systemctl stop mohrs
sudo systemctl restart mohrs
sudo systemctl status mohrs
```

# Cron job
```
sudo crontab -e
```

Edit:
```
0 9 * * * sudo systemctl restart mohrs
```
