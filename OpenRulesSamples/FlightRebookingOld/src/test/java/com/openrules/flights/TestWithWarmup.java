package com.openrules.flights;

import com.openrules.core.Goal;

public class TestWithWarmup {
	
	public static void main(String[] args) {
	    Problem problem = new Problem();
	    // warm up
	    for (int i = 0; i < 100; i++) {
	        execute(problem); 
	    }
        // the actual test
        execute(problem); 
        System.out.println("==== Rebooked Passengers");
        for(Passenger p : problem.passengers) {
            System.out.println(p);
        }
	}
	
	public static void execute(Problem problem) {
	    long startTime = System.nanoTime();
        DecisionModelFlightRebooking model = new DecisionModelFlightRebooking();
        Goal goal = model.createGoal("DetermineFlightRebooking");
        goal.put("trace", "Off");
        goal.put("report", "Off");
        goal.use("Problem", problem);
        goal.execute(); 
        double elapsedTime = (System.nanoTime() - startTime)/1000000; // ms
        System.out.println("Total Elapsed time: " + elapsedTime + " ms");
	}
}


