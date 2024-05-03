package iterate;

import java.util.Map;

import com.openrules.core.util.CLIParser;

public class Test {

    public static void main(String[] args) throws Exception {
        Map<String,String> properties = new CLIParser().parse(args).getOptions();
        DecisionModelIterateTest test = new DecisionModelIterateTest();
        test.runTests(properties);
    }
}
