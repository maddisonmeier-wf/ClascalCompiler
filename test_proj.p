(* shows use of recursive calls *)

program test;

class AttrArray 
BEGIN
    VAR
    arr : ARRAY [1..20] of integer;

    FUNCTION sum(cl: AttrArray ; nn : integer) : integer;
    VAR 
        return, small : integer;
    BEGIN
        if nn = 0 THEN
            return := 0
        ELSE
        BEGIN
            small := sum(cl, nn-1);
            return := small + cl.arr[nn]
        END;
        
        sum := return
    END
END

class test
BEGIN   
VAR
    ii, jj, tmp : integer;
    cl : AttrArray;
FUNCTION test;
BEGIN

  cl := new AttrArray;

  ii := 1;
  while ii <= 20 DO
  BEGIN
    cl.arr[ii] := ii;
    ii := ii + 1
  END;

  ii := 1;
  while ii <= 20 DO
  BEGIN
    tmp := tmp + cl.arr[ii];
    ii := ii + 1
  END;

  print tmp; (*iterative sum *)

  tmp := cl.sum(cl, 20);

  print tmp (*recursive sum *)  

      

END;



FUNCTION fib(VAR xx :integer): integer;
VAR
    return : integer;
    fb1 , fb2 :integer;
BEGIN
    
    if xx < 0 THEN
        BEGIN
            return := -1
        END
    ELSE if xx = 0 THEN
        BEGIN
            return := 0
        END
    ELSE if xx = 1 THEN
        BEGIN
            return := 1
        END
    ELSE 
        BEGIN
        fb1 := fib(xx-1);
        fb2 := fib(xx-2);
        return := fb1 + fb2
        END;

    xx := xx + 1;

    fib := return
END


END
.
