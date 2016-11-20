
all:
	javac -cp po-uilib.jar:.:pex-support.jar `find pex -name *.java`

run:
	java -cp po-uilib.jar:.:pex-support.jar pex.app.App

runtests:
	./runtests.sh
	
clean:
	rm -f `find pex -name "*.class"`
	
jar:
	jar cf proj.jar `find pex -name "*.java"`

