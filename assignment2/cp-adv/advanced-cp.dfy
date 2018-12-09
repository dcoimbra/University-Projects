/*
 * This is the skeleton for your cp utility.
 *
 * Rui Maranhao -- rui@computer.org
 */

include "Io.dfy"

// Useful to convert Dafny strings into arrays of characters.
method ArrayFromSeq<A>(s: seq<A>) returns (a: array<A>)
  ensures a[..] == s
{
  a := new A[|s|] ( i requires 0 <= i < |s| => s[i] );
}

/*predicate equals(a: seq, b: seq)
  requires |a| == |b|
  {
    forall i :: 0 <= i < |a| ==> a[i] == b[i]
  }  
*/ 
method {:main} Main(ghost env: HostEnvironment?)
  requires env != null && env.Valid() && env.ok.ok();
  requires |env.constants.CommandLineArgs()| == 3
  requires env.constants.CommandLineArgs()[1] in env.files.state()
  //requires !(env.constants.CommandLineArgs()[2] in env.files.state())

  modifies env.ok
  modifies env.files

  ensures env.ok.ok() ==> env.constants.CommandLineArgs()[1] in env.files.state()
  ensures env.ok.ok() ==> env.constants.CommandLineArgs()[2] in env.files.state()
  ensures env.ok.ok() ==> 
  |env.files.state()[env.constants.CommandLineArgs()[1]]| == |env.files.state()[env.constants.CommandLineArgs()[2]]|;

  
  ensures env.ok.ok() ==> var src := env.files.state()[env.constants.CommandLineArgs()[1]];
                          var dst := env.files.state()[env.constants.CommandLineArgs()[2]];
                          //forall i :: 0 <= i < |src| ==> src[i] == dst[i]
                          src[..] == dst[..];
{
  var srcName := HostConstants.GetCommandLineArg(1, env);
  var destName := HostConstants.GetCommandLineArg(2, env);

  var srcExists := FileStream.FileExists(srcName, env);
  var dstExists := FileStream.FileExists(destName, env);    


  if dstExists {
    var okToWrite := FileStream.UserInput(env);

    if !okToWrite {
      return;
    } 
  } 

  if srcExists {

    var srcOpenSuccess, srcStream := FileStream.Open(srcName, env);

    if srcOpenSuccess {

      var lengthSuccess, srcLength := FileStream.FileLength(srcName, env);

      if lengthSuccess && srcLength >= 0 {
        
        var buffer: array<byte> := new byte[srcLength];

        var readSuccess := srcStream.Read(0, buffer, 0, srcLength);

        if readSuccess {

          var destOpenSuccess, destStream := FileStream.Open(destName, env);

          if destOpenSuccess {

            if dstExists {
              var cleanSuccess := destStream.CleanFile();

              if !cleanSuccess {
                return;
              }
            }

            var writeSuccess := destStream.Write(0, buffer, 0, srcLength);
          }
        }
      }
    }
  }
  
  print "done!\n";
}