/*******************************************************************************
 * Copyright (c) 2019-2021 OpenRules, Inc.
 *******************************************************************************/
package loan.origination;

import com.openrules.core.DecisionModel;
import com.openrules.core.Goal;


public class Test {

    public static void main(String[] args) {
        
        long startTime = System.currentTimeMillis();

        DecisionModel model = new LoanOrigination();
        Goal goal = model.createGoal();
        
        Applicant[] applicants =  applicantsArray.get();
        RequestedProduct[] requestedProducts =  requestedProductsArray.get();
        Application[] applications =  applicationsArray.get();
        
        for (int i = 0; i < applications.length; i++) {
            System.out.println("\n *** TEST-"+(i+1));
            System.out.println("\t" + applications[i].toString());
            goal.use("Application", applications[i]);
            goal.use("Applicant",applicants[i]);
            goal.use("RequestedProduct",requestedProducts[i]);
            goal.execute();
            
            System.out.println("\tLoan Origination Result: " + applications[i].getLoanOriginationResult());
        }
        
        long endTime = System.currentTimeMillis();
        System.out.println("Total Elapsed time: " + (endTime - startTime +1) + " ms");
 
    }
}
