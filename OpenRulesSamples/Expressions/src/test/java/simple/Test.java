/*******************************************************************************
 * Copyright (c) 2019 OpenRules, Inc.
 *******************************************************************************/
package simple;

import java.util.ArrayList;
import java.util.Map;

import com.openrules.core.Goal;
import com.openrules.core.util.CLIParser;

public class Test {

	public static void main(String[] args) throws Exception {
	    Map<String,String> properties = new CLIParser().parse(args).getOptions();
	    
	    DecisionModelExpressions model = new DecisionModelExpressions();
        Goal goal = model.createGoal();
        Response response = new Response();
        response.setId("Response 1");
        response.setExternalAmounts(new double[] {3.14,78.5,36.6 });
        ArrayList<String> libraryBooks = response.getLibraryBooks();
        libraryBooks.add("Tostoy");
        libraryBooks.add("Faulkner");
        libraryBooks.add("Joice");
        goal.use("Response", response);
        goal.execute();
        System.out.println(response);
        
        DecisionModelExpressionsTest test = new DecisionModelExpressionsTest();
        test.runTests(properties);
	}
} 
