/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package org.simple;

import java.util.Map;

import org.simple.Response;

import com.openrules.core.Goal;
import com.openrules.core.util.CLIParser;


public class Test {

	public static void main(String[] args) throws Exception {
	    
	    Map<String,String> properties = new CLIParser().parse(args).getOptions();
	    
	    DecisionModelAddArrays model = new DecisionModelAddArrays();
        Goal  goal = model.createGoal();
        Response response = new Response();
        response.setId("Response 1");
        response.setExternalAmounts(new double[] {3.14,78.5,36.6 });
        
        response.setLibraryBooks(new String[] {
                "Tostoy", "Faulkner","Joice"});
        goal.use("Response", response);
        goal.execute();
        System.out.println(response);
        
        DecisionModelAddArraysTest test = new DecisionModelAddArraysTest();
        test.runTests(properties); 
	}
} 
