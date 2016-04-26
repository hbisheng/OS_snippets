public class Eisenberg {
	
	public static final int N = 20;
	public static void main(String args[]) {
		
		Thread[] processes = new Thread[Eisenberg.N];
		for(int i = 0; i < Eisenberg.N; i++) {
			processes[i] = new Thread(new Process(i));
		}
		
		Process.initialize();
		for(int i = 0; i < Eisenberg.N; i++) {
			processes[i].start();
		}
		
		try{
			for(int i = 0; i < Eisenberg.N; i++) {	
				processes[i].join();
			}
		} catch(InterruptedException e) {
			e.printStackTrace();
		}
	}
}

class Process implements Runnable {	
	
	enum STATE {
		IDLE,
		WAITING,
		ACTIVE
	};
	static STATE[] flags = new STATE[Eisenberg.N];
	static int turn = 0;
	
	int index;
	int pid = -1;
	public Process(int id) {
		pid = id;
	}
	
	static void initialize(){
		turn = 0;
		for(int i = 0; i < Eisenberg.N; i++) {
			flags[i] = STATE.IDLE;
		}
	}
	
	public void run() {
		while(true){	
			flags[pid] = STATE.WAITING;
			/* scan processes from the one with the turn up to ourselves. */
			/* repeat if necessary until the scan finds all processes idle */
			index = turn;
			while (index != pid) {
				if (flags[index] != STATE.IDLE) {
					index = turn;
				} else {
					index = (index+1) % Eisenberg.N;
				}
			}
			/* now tentatively claim the resource */
			flags[pid] = STATE.ACTIVE;

			/* find the first active process besides ourselves, if any */
			index = 0;
			while ( (index < Eisenberg.N) && ( (index == pid) || (flags[index] != STATE.ACTIVE) )){
				index = index + 1;
			}
			
			if((index >= Eisenberg.N) && ((turn == pid) || (flags[turn] == STATE.IDLE))){
				break;
			}
		}
		
		turn = pid;
		System.out.println("Process " + pid + " entered critical section...");
		doSomething();
		System.out.println("Process " + pid + " exited critical section...\n");
		
		/* find a process which is not IDLE */
		/* (if there are no others, we will find ourselves) */
		index = (turn + 1) % Eisenberg.N;
		while (flags[index] == STATE.IDLE) {
			index = (index + 1) % Eisenberg.N;
		}
		
		/* give the turn to someone that needs it, or keep it */
		turn = index;

		/* we're finished now */
		flags[pid] = STATE.IDLE;
	}
	
	public void doSomething(){
		System.out.println("Process " + pid + " is working...");
	}
}