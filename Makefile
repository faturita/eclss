SRC=src/
FUENTES=$(SRC)server.cpp $(SRC)https.cpp $(SRC)handler.cpp $(SRC)xslprocessor.cpp $(SRC)email.cpp  $(SRC)connection.cpp $(SRC)base64.c
FUENTES2=$(SRC)assassin.c $(SRC)https.cpp 

# -lcliv2

bin/eclss : $(FUENTES)
	g++ -g -ggdb $(FUENTES) -I/usr/local/include/node/ -o bin/eclss  -lpthread -lssl -Lxslt/lib/ -Ixslt/include/ -Ixml2/include/libxml2/ -Lxml2/lib/ -lxslt -D_REENTRANT	
	kill -9 `pgrep eclss` || echo 'no processes'
bin/assassin : $(FUENTES2)
	g++ -g -ggdb $(FUENTES2) -o bin/assassin -lpthread -lssl -D_REENTRANT 
clean :
	rm bin/eclss
	rm -R deploy
pack:
	mkdir deploy
	cp -R bin deploy/
	cp -R xml2 deploy/
	cp -R xslt deploy/
	cp TODO deploy/
	cp README deploy/
	cp INSTALL deploy/
	cp Makefile deploy/
	cp -R src deploy/
	tar cvzf eclss.tgz -C deploy .
	cd ..	
install :
	su root "killall -9 eclss || echo hola"
	su root "cp bin/eclss.xsl /opt/eclss/eclss.xsl"
	su root "cp bin/eclss.xml.prod /opt/eclss/eclss.xml"
	su root "cp bin/eclss /opt/eclss/eclss"
	su root "su daemon \"cd /opt/eclss && ./eclss-start.sh\" "