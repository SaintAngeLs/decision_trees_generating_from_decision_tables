package com.openrules.flights;

import java.util.Map;

import com.openrules.core.Goal;
import com.openrules.core.util.CLIParser;
import com.openrules.flights.Passenger;
import com.openrules.flights.Problem;

public class Main {
	
	public static void main(String[] args) {
	    Map<String, String> arguments = new CLIParser().parse(args).getOptions();
	    System.out.println("ARGUMENTS: " + arguments);
	    Problem problem = new Problem();
	    
	    long startTime = System.currentTimeMillis();
        DecisionModelFlightRebooking model = new DecisionModelFlightRebooking();
        Goal goal = model.createGoal("DetermineFlightRebooking");
        goal.put("trace", "On");
        goal.put("report", "On");
        goal.use("Problem", problem);
        goal.execute(); 
        long endTime = System.currentTimeMillis();
        System.out.println("Total Elapsed time: " + (endTime - startTime +1) + " ms");
        
        System.out.println("==== Rebooked Passengers");
        for(Passenger p : problem.passengers) {
            System.out.println(p);
        }
        
	}
	
}


