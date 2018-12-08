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

predicate same_bytes(b1: byte, b2: byte)
{
  b1 == b2
}


function build_same_bytes (bytes:seq<byte>, pos: int, counter: byte) : seq<byte>
requires |bytes| > 0
requires 0 <= pos < |bytes|
requires counter >= 0
decreases |bytes| - pos
{
  if |bytes| == 1 then bytes
  else if pos == 0 then [bytes[0]] + build_same_bytes(bytes, 1, 1)
  else if 0 < pos < |bytes| - 1 && same_bytes(bytes[pos - 1], bytes[pos]) && counter == 255 then [bytes[pos]] + build_same_bytes(bytes, pos + 1, 1)
  else if 0 < pos < |bytes| - 1 && !same_bytes(bytes[pos - 1], bytes[pos]) then [bytes[pos]] + build_same_bytes(bytes, pos + 1, 1)
  else if pos == |bytes| - 1 && same_bytes(bytes[pos - 1], bytes[pos]) && counter == 255 then [bytes[pos]]
  else if pos == |bytes| - 1 && same_bytes(bytes[pos - 1], bytes[pos]) && counter != 255 then []
  else if pos == |bytes| - 1 && !same_bytes(bytes[pos - 1], bytes[pos]) then [bytes[pos]]
  else build_same_bytes(bytes, pos + 1, counter + 1)
}


function count_bytes (bytes:seq<byte>, pos: int, counter: byte) : seq<byte>
requires |bytes| > 0
requires 0 <= pos < |bytes|
requires counter >= 0
decreases |bytes| - pos
{
  if |bytes| == 1 then [1]
  else if pos == 0 then count_bytes(bytes, 1, 1)
  else if 0 < pos < |bytes| - 1 && same_bytes(bytes[pos - 1], bytes[pos]) && counter == 255 then [counter] + count_bytes(bytes, pos + 1, 1)
  else if 0 < pos < |bytes| - 1 && !same_bytes(bytes[pos - 1], bytes[pos]) then [counter] + count_bytes(bytes, pos + 1, 1)
  else if pos == |bytes| - 1 && same_bytes(bytes[pos - 1], bytes[pos]) && counter == 255 then [1]
  else if pos == |bytes| - 1 && same_bytes(bytes[pos - 1], bytes[pos]) && counter != 255 then [counter + 1]
  else if pos == |bytes| - 1 && !same_bytes(bytes[pos - 1], bytes[pos]) then [1]
  else count_bytes(bytes, pos + 1, counter + 1)
}

/*function compress(bytes:seq<byte>) : seq<byte>
{
  if |bytes| <= 0 then []
  else build_same_bytes(bytes, 0, 0) + [0] + count_bytes(bytes, 0, 0)
}
*/
function decompress(bytes:seq<byte>) : seq<byte>
{
  bytes
}

lemma lossless(bytes:seq<byte>)
 // ensures decompress(compress(bytes)) == bytes;
{
}


lemma {:axiom true} eq_result(bytes: seq<byte>)
requires |bytes| > 1
requires bytes[|bytes|-1] == bytes[|bytes|-2]
ensures build_same_bytes(bytes, 0, 0) == build_same_bytes(bytes[..|bytes|-1], 0, 0) 



lemma {:axiom true} split_result(bytes: seq<byte>, next: byte, ctr: byte)
requires |bytes| > 0
requires bytes[|bytes|-1] != next || ctr == 255
ensures build_same_bytes(bytes + [next], 0, 0) == build_same_bytes(bytes, 0, 0) +  build_same_bytes([next], 0, 0)


/*lemma {:axiom true} lemma_bytes (bytes: seq<byte>, next: byte, ctr:byte)
requires |bytes| > 0
ensures bytes[|bytes|-1] == next && ctr < 255 ==> build_same_bytes(bytes + [next], 0, 0) == build_same_bytes(bytes, 0, 0) 
ensures bytes[|bytes|-1] != next || ctr == 255 ==> build_same_bytes(bytes + [next], 0, 0) == build_same_bytes(bytes, 0, 0) +  build_same_bytes([next], 0, 0)
*/

method get_runs(bytes:array?<byte>) returns (runs:seq<byte>, counts:seq<byte>)
  requires bytes != null
  ensures bytes.Length > 0 ==> |runs| == |counts|
  ensures bytes.Length > 0 ==> runs == build_same_bytes(bytes[..], 0, 0)
  //ensures bytes.Length > 0 ==> counts == count_bytes(bytes[..], 0, 0)
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
  
  assert runs == build_same_bytes(bytes[..pos], 0, 0);


  while pos < bytes.Length
    invariant 0 <= pos <= bytes.Length
    invariant count >= 0
    invariant 0 <= run_idx < |runs|
    invariant |runs| == |counts|
    invariant runs == build_same_bytes(bytes[..pos], 0, 0)
    decreases bytes.Length - pos 
  {

    if bytes[pos] != bytes[pos-1] || count == 255
    {
      calc == {
            runs + [bytes[pos]];
          == build_same_bytes(bytes[..pos],0,0) + [bytes[pos]];
          == { split_result(bytes[..pos], bytes[pos], count); }
            build_same_bytes(bytes[..pos] + [bytes[pos]],0,0);
          == { assert bytes[..pos+1] == bytes[..pos] + [bytes[pos]]; }
            build_same_bytes(bytes[..pos+1], 0, 0);
      }
    }
    else {
        calc == {
            runs;
          == build_same_bytes(bytes[..pos],0,0);
          == { eq_result(bytes[..pos] + [bytes[pos]]); }
           build_same_bytes(bytes[..pos] + [bytes[pos]],0,0);
          == { assert bytes[..pos+1] == bytes[..pos] + [bytes[pos]]; }
            build_same_bytes(bytes[..pos+1], 0, 0);
      }
    }

    if current_byte == bytes[pos] {

      if count == 255 {
        count := 0;

        runs := runs + [bytes[pos]];
        counts := counts + [1];

        run_idx := run_idx + 1;
      }

      count := count + 1;
      runs := runs;
      
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

  assert runs == build_same_bytes(bytes[..(bytes.Length)],0,0);

  assert bytes[..] == bytes[..bytes.Length];
}

method compress_impl(bytes:array?<byte>) returns (compressed_bytes:array?<byte>)
  requires bytes != null;
  ensures  compressed_bytes != null;
  // ensures  compressed_bytes[..] == compress(bytes[..]);
{
  var runs, counts := get_runs(bytes);

  counts := [0] + counts;

  compressed_bytes := ArrayFromSeq<byte>(runs + counts);
}



method redo_original(sequence:seq<byte>, counters:seq<byte>) returns (original_bytes:array?<byte>)
requires |sequence| > 0
requires |counters| > 0
requires |sequence| == |counters|
ensures original_bytes != null

{

  //Initialized with first element, gonna be subtracted to the counter later
  var original_aux : seq<byte> := [sequence[0]];

  var pos := 0;
  var len := |sequence|;

  while pos < len
  invariant 0 <= pos <= len;
  decreases len-pos
  {
    var byte := sequence[pos];
    var counter := counters[pos] as int;

    //The first element was used to initialize the sequence
    if pos == 0 {
      counter := counter - 1;
    }

    while counter > 0
    decreases counter
    { 
      original_aux := original_aux + [byte];
      counter := counter - 1;
    }
    
    pos := pos + 1;
  }

  original_bytes := ArrayFromSeq<byte>(original_aux);
}



method find_separator_index(bytes:array?<byte>) returns (index:int)
requires bytes != null
{
  index := bytes.Length - 1;
  
  while index >= 0
  decreases index
  {
    if bytes[index] == 0 {
      return;
    }

    else {
      index := index - 1;
    }
  }

  index := -1; //file doesn't have 0, so can't be decompressed
}

method decompress_impl(compressed_bytes:array?<byte>) returns (decompressed_bytes:array?<byte>)
  requires compressed_bytes != null
  ensures  decompressed_bytes != null;
  //ensures  decompressed_bytes[..] == decompress(compressed_bytes[..]);
{

  var idx := find_separator_index(compressed_bytes);

  if idx < compressed_bytes.Length && idx >= 0 {
    var sequence := compressed_bytes[..idx];
    var counters := compressed_bytes[(idx+1)..];

    if |sequence| > 0 && |counters| > 0 && |sequence| == |counters| { 
      decompressed_bytes := redo_original(sequence, counters);
      return;
    }
  }

  //If it didn't find the separator index, means that the file can't be decompressed
  decompressed_bytes := compressed_bytes;
}

method {:main} Main(ghost env:HostEnvironment?)
  requires env != null && env.Valid() && env.ok.ok();
  requires |env.constants.CommandLineArgs()| == 4
  requires env.constants.CommandLineArgs()[2] in env.files.state()
  requires !(env.constants.CommandLineArgs()[3] in env.files.state())

  requires env.constants.CommandLineArgs()[1] == "0" || env.constants.CommandLineArgs()[1] == "1"

  modifies env.ok
  modifies env.files

  ensures env.ok.ok() ==> env.constants.CommandLineArgs()[2] in env.files.state() && env.constants.CommandLineArgs()[3] in env.files.state()
{

   var srcName := HostConstants.GetCommandLineArg(2, env);
   var destName := HostConstants.GetCommandLineArg(3, env);

  var srcExists := FileStream.FileExists(srcName, env);

  if srcExists {

    var srcOpenSuccess, srcStream := FileStream.Open(srcName, env);
 
    if srcOpenSuccess {

      var srcLengthSuccess, srcLength := FileStream.FileLength(srcName, env);

      if srcLengthSuccess && srcLength >= 0 {
        
        var buffer: array<byte> := new byte[srcLength];

        var readSuccess := srcStream.Read(0, buffer, 0, srcLength);

        var compOrDecomp := HostConstants.GetCommandLineArg(1, env);

        var result: array?<byte> := new byte[1];
        result[0] := 0;

        if compOrDecomp[0] == '1' {
          print "Compress me!\n";
          result := compress_impl(buffer);
        }

        else {
          print "Decompress me!\n";
          result := decompress_impl(buffer);
        }


        if readSuccess {

          var destOpenSuccess, destStream := FileStream.Open(destName, env);

          if destOpenSuccess {

            if -0x80000000 <= result.Length < 0x80000000 {

              var writeSuccess := destStream.Write(0, result, 0, result.Length as int32);
            }
          }
        }
      }
    }
  }
  print "done!\n";
}