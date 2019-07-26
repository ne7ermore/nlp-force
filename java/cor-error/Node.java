import java.util.HashMap;

@Data
public class Node {
    private String word;
    private double hot;
    private HashMap<Integer, Node> children = new HashMap<>();

    public Node() {};

    public Node(String word, double hot) {
        this.hot = hot;
        this.word = word;
    }
}