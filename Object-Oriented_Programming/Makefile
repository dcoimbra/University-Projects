
all:
	javac -cp po-uilib.jar:.:pex-support.jar `find pex -name *.java`

run:
	java -cp po-uilib.jar:.:pex-support.jar pex.app.App

runtests:
	./runtests.sh
	
clean:
	rm -f `find pex -name "*.class"`
	
clean1:
	rm -f `find pex -name "Label.*"`
	rm -f `find pex -name "Message.*"`
	
jar:
	make clean1
	jar cvf proj.jar `find pex -name "*.java"`

extract:
	jar xvf proj.jar

doc:
	javadoc -d docs `find pex -name "*.java"`