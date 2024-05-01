package healthcare.service;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.node.ObjectNode;
import org.springframework.stereotype.Component;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

@Component
public class ErrorCodeHandler implements PatientTherapyRestControllerPostHandler {

    public JsonNode postHandle(HttpServletRequest httpRequest, HttpServletResponse httpResponse,
            JsonNode responsePayload) throws IOException {

        JsonNode errorMessages = responsePayload.at("/response/doctorVisit/errorMessages");
        if (!errorMessages.isEmpty()) {
            httpResponse.setStatus(HttpServletResponse.SC_BAD_REQUEST);
            httpResponse.addHeader("X-Error-Codes", errorMessages.toString());
            ((ObjectNode) responsePayload).put("decisionStatusCode", 400);
        }
        return responsePayload;
    }

}
