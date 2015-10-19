program testit

character*200 :: line
character*5, allocatable, dimension(:) :: stations
character*2, allocatable, dimension(:) :: states
integer :: numpts, errval

call system("mkdir /tmp/pounds")
call system("mkdir /tmp/pounds/data")
call system("wget -O /tmp/pounds/data/station.txt -q http://theochem.mercer.edu/csc330/data/station.txt") 
call system("wget -O /tmp/pounds/data/daily.txt -q http://theochem.mercer.edu/csc330/data/daily.txt") 

open (unit=15, file='/tmp/pounds/data/station.txt', status='old')

! Count the number of lines in the file so we can allocate space for them
numpts = 0 
errval = 1
do while ( errval /= -1 )
   read (15,'(a5)',iostat=errval) stationid 
   if ( errval  == 0 ) then
      numpts = numpts + 1
   else
    rewind(15)
   end if
end do

allocate (stations(numpts), stat=errval)
if (errval /= 0) stop "Error in stations array allocation"
allocate (states(numpts), stat=errval)
if (errval /= 0) stop "Error in state array allocation"

numpts = 0
errval = 1
do while ( errval /= -1 )
   read (15,'(a200)',iostat=errval) line  
   if ( errval  == 0 ) then
      numpts = numpts + 1
      ! Find location of first delimiter
      linepos = 0
      do while ( (line(linepos:linepos) .ne. '|') .and. ( linepos .le. 200 ) ) 
         linepos = linepos + 1;
      end do

      ! Put the substring into an array
      stations(numpts) = line(1:linepos-1) 

      ! Now go find the state by looking for 7th delimiter 
      linepos = linepos + 1

      do i = 1, 6
         do while ( (line(linepos:linepos) .ne. '|') .and. ( linepos .le. 200 ) )
            linepos = linepos + 1
         end do
         linepos = linepos + 1
      enddo

      ! Now that we are one space beyond the 7th delimiter, store it and move to
      ! find the next one.  The substring between them should contain the state.
      linestart = linepos
      do while ( (line(linepos:linepos) .ne. '|') .and. ( linepos .le. 200 ) ) 
         linepos = linepos + 1;
      end do

      ! Stick it in an array
      states(numpts) = line(linestart:linepos) 
   else
    close(15)
   end if
end do

do i=1, numpts
   print *, stations(i), ' ', states(numpts)
enddo

call system("rm -rf /tmp/pounds")

end program
