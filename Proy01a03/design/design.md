Pseudocode:

```cpp
shared number_queue
shared sums_queue
shared thread_response_queues

WebServer:
  for 1 to processesor_num
    start_thread(AssemblerCalculator)
  end for
  
  start_thread(SumsDispatcher)
  
  start_thread(number_of_requests, Connection(request))
  
  
Connection(numbers_requests):
  for i := 1 to numbers_requests.size()
    number_queue.push_back(numbers_requests[i])
  end for
  
  for 1 to request_count
    sums := thread_response_queues[thread_number].pop()
  end for
  
  vista.show(sums)
  
AssemblerCalculator:
  while !stop_condition
    number := number_queue.pop()
    sums := getGolbachSums(number)
    sums_queue.push_back(sums)
  end while

SumsDispatcher:
  while !stop_condition
    response := sums_queue.pop()
    thread_response_queues[thread_number].push_back(response)
  end while
```

