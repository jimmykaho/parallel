Each of the thread has a different computation time and increases as its thread number increases.
It results in such behavior because we assign work load proportionally to each thread's thread number.
Therefore, the higher the thread number, the more work is assigned to the thread.
By looking at the profiles generated, we can conclude that the thread with more assigned work load need more
time to finish computation than the ones with less. This result in a load imbalance such that the ones that 
finish early, they have to wait for the one(s) still in computation before processing. We can see that thread
3 spent the longest time in "doComputation" with an exclusive time of 2,828msec; and hence, other threads are
dependent on thread 3.
Im order to optimize resource use, we need to think about how to balance the work load across the limited
resource that provided to us.
