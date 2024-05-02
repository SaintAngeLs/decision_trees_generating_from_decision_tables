/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package com.openrules.flights;

import java.util.Arrays;

import com.openrules.core.Decision;
import com.openrules.flights.Passenger;
import com.openrules.flights.Problem;

public class MainDecision {
	
	public static void main(String[] args) {

	    DecisionModelFlightRebooking model = new DecisionModelFlightRebooking();
        Decision decision = model.getDecision();
        
        Problem problem = new Problem();
        decision.put("Problem", problem);
        decision.useBusinessObject("Problem", problem);
        decision.execute("DetermineFlightRebooking"); 
        
        System.out.println("==== Rebooked Passengers");
        for(Passenger p : problem.passengers) {
            System.out.println(p);
        }
	}
}


