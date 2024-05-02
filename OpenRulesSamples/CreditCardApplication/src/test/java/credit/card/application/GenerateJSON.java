package credit.card.application;

import java.io.File;

import com.fasterxml.jackson.databind.ObjectMapper;


public class GenerateJSON {
    
    private static ObjectMapper mapper = new ObjectMapper(); 
    
    public static void main(String[] args) throws Exception {
        
        Applicant[] applicants = applicantsArray.get();
        mapper.writerWithDefaultPrettyPrinter().writeValue(new File("data/Applicants.json"), applicants);
        Application[] applications = applicationsArray.get();
        mapper.writerWithDefaultPrettyPrinter().writeValue(new File("data/Applications.json"), applications);
        
      }

}
