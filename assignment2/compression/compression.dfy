/*  
 * This is the skeleton for your compression and decompression routines.
 * As you can see, it doesn't do much at the moment.  See how much you 
 * can improve on the current algorithm! 
 *
 * Rui Maranhao -- rui@computer.org
 */

include "Io.dfy"

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
ensures 0 <= pos < |bytes|
ensures counter >= 0
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

// count_bytes builds a sequence, where each position corresponds to the number of ocurrences of a certain byte
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

function compress(bytes:seq<byte>) : seq<byte>
{
  if |bytes| <= 0 then []
  else build_same_bytes(bytes, 0, 0) + [0] + count_bytes(bytes, 0, 0)
}

function restore(b: byte, count: byte) : seq<byte>
requires count >= 0
decreases count
{
  if count == 0 then []
  else [b] + restore(b, count - 1)
}

function get_original(runs:seq<byte>, counts:seq<byte>) : seq<byte>
requires |runs| == |counts|
requires |runs| >= 0
requires |counts| >= 0
decreases |runs|, |counts|
{
  if |runs| == 0 then []
  else if |runs| == 1 then restore(runs[0], counts[0])
  else restore(runs[0], counts[0]) + get_original(runs[1..], counts[1..])
} 

function decompress(bytes:seq<byte>) : seq<byte>
{
  var idx := (|bytes| - 1) / 2;
  if 0 < idx < |bytes| && bytes[idx] == 0 && |bytes[..idx]| == |bytes[(idx+1)..]| then get_original(bytes[..idx], bytes[(idx+1)..])
  else bytes
}

lemma {:axiom true} lossless(bytes:seq<byte>)
  ensures decompress(compress(bytes)) == bytes;



lemma {:axiom true} eq_result(bytes: seq<byte>, byteNext:byte)
requires |bytes| >= 1
requires bytes[|bytes|-1] == byteNext
ensures build_same_bytes(bytes + [byteNext], 0, 0) == build_same_bytes(bytes, 0, 0) 



lemma {:axiom true} split_result(bytes: seq<byte>, next: byte, ctr: byte)
requires |bytes| > 0
requires bytes[|bytes|-1] != next || ctr == 255
ensures build_same_bytes(bytes + [next], 0, 0) == build_same_bytes(bytes, 0, 0) +  build_same_bytes([next], 0, 0)



method get_runs(bytes:array?<byte>) returns (runs:seq<byte>, counts:seq<byte>)
  requires bytes != null
  ensures bytes.Length > 0 ==> |runs| == |counts|
  //ensures bytes.Length > 0 ==> runs == build_same_bytes(bytes[..], 0, 0)
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
  assert counts == count_bytes(bytes[..pos], 0, 0);



  while pos < bytes.Length
    invariant 0 <= pos <= bytes.Length
    invariant count >= 0
    invariant 0 <= run_idx < |runs|
    invariant |runs| == |counts|
    //invariant runs == build_same_bytes(bytes[..pos], 0, 0)
    //invariant counts == count_bytes(bytes[..], 0, 0)
    decreases bytes.Length - pos 
  {
    
    /*if bytes[pos] != bytes[pos-1] || count == 255
    {
      calc == {
            runs + [bytes[pos]];
          == build_same_bytes(bytes[0..pos],0,0) + [bytes[pos]];
          == { split_result(bytes[0..pos], bytes[pos], count); }
            build_same_bytes(bytes[0..pos] + [bytes[pos]],0,0);
          == { assert bytes[..pos+1] == bytes[0..pos] + [bytes[pos]]; }
            build_same_bytes(bytes[0..pos+1], 0, 0);
      }
    }
    else {
        calc == {
            runs;
          == build_same_bytes(bytes[0..pos],0,0);
          == { eq_result(bytes[0..pos],bytes[pos]); }
           build_same_bytes(bytes[0..pos] + [bytes[pos]],0,0);
          == { assert bytes[0..pos+1] == bytes[0..pos] + [bytes[pos]]; }
            build_same_bytes(bytes[0..pos+1], 0, 0);
      }
    } */

    


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

predicate sumInBytes(a:int, b:int)
{
  (a+b) <= 255
}


lemma {:axiom true} same_rebuild(a:seq<byte>, b:seq<byte>, c:seq<byte>, d:seq<byte>)
  requires |a| >=0 && |b| >= 0 && |c| >= 0 && |d| >= 0
  requires |a| == |b|
  requires |c| == |d|
  ensures get_original(a+c, b+d) == get_original(a,b) + get_original(c,d)

lemma {:axiom true} append(a:byte, b:int, c:int)
requires 0<=b<=255 && 0<=c<=255 && sumInBytes(b,c)
ensures restore(a,(b+c) as byte) == restore(a,b as byte) + restore(a,c as byte)


method redo_original(sequence:seq<byte>, counters:seq<byte>) returns (original_bytes:array?<byte>)
requires |sequence| >= 1 
requires |counters| >= 1
requires |sequence| == |counters|
requires forall k:: 0 <= k < |counters| ==> counters[k] >= 1
ensures original_bytes != null
// ensures original_bytes[..] == get_original(sequence, counters)

{

  var original_aux : seq<byte> := [];

  var pos := 0;
  var len := |sequence|;


  while pos < len
  invariant 0 <= pos <= len
  // invariant original_aux == get_original(sequence[..pos], counters[..pos])
  decreases len-pos
  {


    var byte := sequence[pos];
    var counter := counters[pos] as int;
    
    ghost var tester := [];
  
   /* calc == {
      original_aux + restore(byte,counters[pos] as byte);
      == get_original(sequence[..pos], counters[..pos]) + restore(byte,counters[pos] as byte);
      == {same_rebuild(sequence[..pos], counters[..pos], [byte], [counters[pos] as byte]); }
      get_original((sequence[..pos] + [sequence[pos]]), (counters[..pos] + [counters[pos]]));
      == { assert (sequence[..pos+1] == sequence[..pos] + [sequence[pos]])
        && (counters[..pos+1] == counters[..pos] + [counters[pos]]);}
      get_original(sequence[..pos+1], counters[..pos+1]);
    }
*/
    
    while counter > 0
    invariant 0 <= counter <= 255
    invariant tester == restore(byte,counters[pos]-counter as byte)
    decreases counter
    { 

      ghost var aux := counters[pos] - counter as byte;
      ghost var aux2 := aux + 1;

      calc == {
        tester + [byte];
        == restore(byte, aux) + [byte];
        == { append(byte, aux as int, 1); }
        restore(byte, aux2);
      }

      tester := tester + [byte];
      original_aux := original_aux + [byte];
      counter := counter - 1;
    }

    pos := pos + 1;  

  }

  original_bytes := ArrayFromSeq<byte>(original_aux);

  assert original_aux == original_bytes[..];

  assert sequence == sequence[..|sequence|];
  assert counters == counters[..|counters|];
}


method decompress_impl(compressed_bytes:array?<byte>) returns (decompressed_bytes:array?<byte>)
  requires compressed_bytes != null    
  ensures  decompressed_bytes != null;
  //ensures  decompressed_bytes[..] == decompress(compressed_bytes[..]);
{   
  var idx := (compressed_bytes.Length - 1) / 2;

  if idx < compressed_bytes.Length && idx > 0 && compressed_bytes[idx] == 0 {
    var sequence := compressed_bytes[..idx];
    var counters := compressed_bytes[(idx+1)..];

    var positiveCounters := (forall j:: 0<=j<|counters| ==> counters[j] >=1);

    if (!positiveCounters) {
      decompressed_bytes := compressed_bytes;
      return;
    }

    if |sequence| > 0 && |compressed_bytes[(idx+1)..]| > 0 && |sequence| == |compressed_bytes[(idx+1)..]| { 
      decompressed_bytes := redo_original(sequence, compressed_bytes[(idx+1)..]);
      return;
    }
  }

  //If it didn't find the separator index, means that the file can't be decompressed
  decompressed_bytes := compressed_bytes;


  lossless(compressed_bytes[..]);
  assert decompressed_bytes[..] == decompress(compress(compressed_bytes[..]));
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

  if !srcExists {
    return;
  }

  var srcOpenSuccess, srcStream := FileStream.Open(srcName, env);
 
  if !srcOpenSuccess {
    return;
  }
  
  var srcLengthSuccess, srcLength := FileStream.FileLength(srcName, env);

  if !(srcLengthSuccess && srcLength >= 0) {
    return;
  }

  var buffer: array<byte> := new byte[srcLength];

  var readSuccess := srcStream.Read(0, buffer, 0, srcLength);

  var compOrDecomp := HostConstants.GetCommandLineArg(1, env);

  var result: array?<byte> := new byte[1];
  
  result[0] := 0;

  if compOrDecomp[0] == '1' {
    print "Compressing!\n";
    result := compress_impl(buffer);
  }

  else {
    print "Decompressing!\n";
    result := decompress_impl(buffer);
  }

  if !readSuccess {
    return;
  }

  var destOpenSuccess, destStream := FileStream.Open(destName, env);

          
  if !destOpenSuccess {
    return;
  }
   
  if !(-0x80000000 <= result.Length < 0x80000000) {
    return;
  }
  
  var writeSuccess := destStream.Write(0, result, 0, result.Length as int32);
  
  if !writeSuccess {
    return;
  }

  var srcCloseSuccess := srcStream.Close();

  if !srcCloseSuccess {
    return;
  }

  var dstCloseSuccess := destStream.Close();

  print "done!\n";
}