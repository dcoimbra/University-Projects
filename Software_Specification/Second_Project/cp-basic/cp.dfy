/*
 * This is the skeleton for your cp utility.
 *
 * Rui Maranhao -- rui@computer.org
 */

include "Io.dfy"

predicate equals(a: seq, b: seq)
  requires |a| == |b|
  {
    forall i :: 0 <= i < |a| ==> a[i] == b[i]
  } 

method {:main} Main(ghost env: HostEnvironment?)
  requires env != null && env.Valid() && env.ok.ok();
  requires |env.constants.CommandLineArgs()| == 3
  requires env.constants.CommandLineArgs()[1] in env.files.state()
  requires !(env.constants.CommandLineArgs()[2] in env.files.state())

  modifies env.ok
  modifies env.files

  ensures env.ok.ok() ==> env.constants.CommandLineArgs()[1] in env.files.state() && env.constants.CommandLineArgs()[2] in env.files.state()
  ensures env.ok.ok() ==> var src := env.files.state()[env.constants.CommandLineArgs()[1]];
                          var dest := env.files.state()[env.constants.CommandLineArgs()[2]];
                          |src| == |dest| 
  
  ensures env.ok.ok() ==> equals(env.files.state()[env.constants.CommandLineArgs()[1]], env.files.state()[env.constants.CommandLineArgs()[2]])
{
  var srcName := HostConstants.GetCommandLineArg(1, env);
  var destName := HostConstants.GetCommandLineArg(2, env);

  var srcExists := FileStream.FileExists(srcName, env);

  if !srcExists {
    return;
  }
  
  var srcOpenSuccess, srcStream := FileStream.Open(srcName, env);

  if !srcOpenSuccess {
    return;
  }

  var lengthSuccess, srcLength := FileStream.FileLength(srcName, env);

  if !(lengthSuccess && srcLength >= 0) {
    return;
  }

  var buffer: array<byte> := new byte[srcLength];

  var readSuccess := srcStream.Read(0, buffer, 0, srcLength);

  if !readSuccess {
    return;
  }

  var destOpenSuccess, destStream := FileStream.Open(destName, env);

  if !destOpenSuccess {
    return;
  }

  var writeSuccess := destStream.Write(0, buffer, 0, srcLength);

  if !writeSuccess {
    return;
  }
              
  var closeSuccessSrc := srcStream.Close();

  if !closeSuccessSrc{
    return;
  }
  
  var closeSuccessDst := destStream.Close();
  
  print "done!\n";
}