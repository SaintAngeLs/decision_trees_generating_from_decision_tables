package hello.java;

import java.util.Calendar;

import com.openrules.core.DecisionModel;
import com.openrules.core.Goal;

public class TestJava {

    public static void main(String[] args) {
        DecisionModel model = new DecisionModelHello();
        Goal goal = model.createGoal();
        goal.put("trace", "On");
        goal.put("report", "On");
        Customer customer = new Customer();
        customer.setName("Robert Smith");
        customer.setGender("Male");
        customer.setMaritalStatus("Single");
        int hour = Calendar.getInstance().get(Calendar.HOUR_OF_DAY);
        customer.setCurrentHour(hour);
        goal.use("Customer", customer);
        goal.execute();
        System.out.println(customer);
    }
}
