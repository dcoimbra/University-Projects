/*  
 * This is the skeleton for your compression and decompression routines.
 * As you can see, it doesn't do much at the moment.  See how much you 
 * can improve on the current algorithm! 
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

function compress(bytes:seq<byte>) : seq<byte>
{
  bytes
}

function decompress(bytes:seq<byte>) : seq<byte>
{
  bytes
}

lemma lossless(bytes:seq<byte>)
  ensures decompress(compress(bytes)) == bytes;
{
}

method get_runs(bytes:array?<byte>) returns (runs:seq<byte>, counts:seq<byte>)
  requires bytes != null;
  ensures bytes.Length > 0 ==> |runs| == |counts|
  {
  if bytes.Length <= 0 {
    return;
  }

  runs := [bytes[0]];
  counts := [1];

  var run_idx := 0;

  var current_byte := bytes[0];
  var count := 1;

  var pos := 1;
  
  while pos < bytes.Length 
    invariant 0 <= pos <= bytes.Length;
    invariant 0 <= run_idx < |runs|;
    invariant |runs| == |counts|
    decreases bytes.Length - pos; 
  {
    if current_byte == bytes[pos] {

      if count == 255 {
        runs := runs + [bytes[pos]];
        counts := counts + [1];

        run_idx := run_idx + 1;
        count := 0;
      }

      count := count + 1;
    }

    else {
      runs := runs + [bytes[pos]];
      counts := counts + [1];

      run_idx := run_idx + 1;
      count := 1;
    }
  
    current_byte := bytes[pos];
    pos := pos + 1;

    counts := counts[run_idx := count];
  }
}

method compress_impl(bytes:array?<byte>) returns (compressed_bytes:array?<byte>)
  requires bytes != null;
  ensures  compressed_bytes != null;
  ensures  compressed_bytes[..] == compress(bytes[..]);
{
  var runs, counts := get_runs(bytes);

  counts := [0] + counts;

  compressed_bytes := ArrayFromSeq<byte>(runs + counts);
}

method decompress_impl(compressed_bytes:array?<byte>) returns (bytes:array?<byte>)
  requires compressed_bytes != null;
  ensures  bytes != null;
  ensures  bytes[..] == decompress(compressed_bytes[..]);
{
  bytes := compressed_bytes;
}

method {:main} Main(ghost env:HostEnvironment?)
  requires env != null && env.Valid() && env.ok.ok();
  requires |env.constants.CommandLineArgs()| == 3
  requires env.constants.CommandLineArgs()[1] in env.files.state()
  requires !(env.constants.CommandLineArgs()[2] in env.files.state())

  modifies env.ok
  modifies env.files

  ensures env.ok.ok() ==> env.constants.CommandLineArgs()[1] in env.files.state() && env.constants.CommandLineArgs()[2] in env.files.state()
{
  print "Compress me!\n";

   var srcName := HostConstants.GetCommandLineArg(1, env);
   var destName := HostConstants.GetCommandLineArg(2, env);

  var srcExists := FileStream.FileExists(srcName, env);

  if srcExists {

    var srcOpenSuccess, srcStream := FileStream.Open(srcName, env);

    if srcOpenSuccess {

      var srcLengthSuccess, srcLength := FileStream.FileLength(srcName, env);

      if srcLengthSuccess && srcLength >= 0 {
        
        var buffer: array<byte> := new byte[srcLength];

        var readSuccess := srcStream.Read(0, buffer, 0, srcLength);

        var compressed := compress_impl(buffer);

        if readSuccess {

          var destOpenSuccess, destStream := FileStream.Open(destName, env);

          if destOpenSuccess {

            if -0x80000000 <= compressed.Length < 0x80000000 {

              var writeSuccess := destStream.Write(0, compressed, 0, compressed.Length as int32);
            }
          }
        }
      }
    }
  }
  print "done!\n";
}