# Medal of Honor - Rising Sun Matchmaker server

This is a project to revive the online matchmaking services. This allows you to go online again. 

## DNS

To let this server work there are some DNS changes required before it all works.
The following domains needs to be redirected to this server:

	*.dnas.playstation.org
	*.theater.ea.com

Read [here](dns/Readme.md) more about it how to install your own DNS.


## Clone Project

```
git clone https://github.com/Project-Backstab/MOHRS-Matchmaker.git
cd MOHRS-Matchmaker

git submodule init
git submodule update

sudo apt-get install doxygen graphviz
```

## Compile project

```
./make_release_build.sh
```
This will create the mohrs executable and copy the data to the "MOHRS-Matchmaker-release" directory.

For development:
```
cd MOHRS-Matchmaker
mkdir build
cd build

cmake ..
cmake --build . --target doc
make -j 4
```

## Firewall

```
sudo ufw allow 80/tcp
sudo ufw allow 8080/tcp
sudo ufw allow 14300/tcp
```

If you want to avoid to run the application as sudo you must Preroute port 80 to 8080
```
sudo iptables -A PREROUTING -t nat -i <network interface> -p tcp --dport 80 -j REDIRECT --to-port 8080
```

## Run

For release:
```
cd MOHRS-Matchmaker-release/
./bin/mohrs
```

For development:
```
cd MOHRS-Matchmaker/build
./bin/mohrs
```

For development debugging:
```
cd MOHRS-Matchmaker/build
gdbserver --multi :2345 bin/mohrs
```

## Services

If you want to run this project as a services you can read more about it over [here](service/Readme.md).