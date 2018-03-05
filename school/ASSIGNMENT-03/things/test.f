      PROGRAM TEST
      implicit none
      integer IA, IB
      IA = 2
      IB = 1
 1000 format (I1,A,I1)

      if (IA .lt. IB) then
         print 1000, IA, ' is less than ', IB
      elseif (IA .eq. IB) then
         print 1000, IA, ' is equal to ', IB
      else
         print 1000, IA, ' is greater than ', IB
      endif

      END PROGRAM TEST
