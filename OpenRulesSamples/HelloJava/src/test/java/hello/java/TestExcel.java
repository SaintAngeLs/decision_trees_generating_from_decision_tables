package hello.java;

import com.openrules.core.DecisionModel;
import com.openrules.core.Goal;

public class TestExcel {

    public static void main(String[] args) {
        DecisionModel model = new DecisionModelHello();
        Goal goal = model.createGoal();
        Customer c = customersArray.get()[0];
        goal.use("Customer", c);
        goal.execute();
        System.out.println(c);
    }
}
