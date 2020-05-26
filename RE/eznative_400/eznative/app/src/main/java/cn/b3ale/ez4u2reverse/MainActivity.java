package cn.b3ale.ez4u2reverse;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public boolean check(String a, String b) {
        Log.d("MY_TAG", String.valueOf(a.equals(b)));
        if (a.equals(b)) {
            return true;
        } else {
            return false;
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button button = (Button)findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                EditText editText = (EditText)findViewById(R.id.editText);
                String txt = editText.getText().toString();
                Log.d("MY_TAG", txt);
                int len = txt.length() / 8, padding, i;
                if (txt.length() % 8 != 0) {
                    padding = 8 - (txt.length() - len * 8);
                    for (i = 0; i < padding; i++)
                        txt += 'x';
                    len += 1;
                }
                Log.d("MY_TAG", txt);
                Log.d("MY_TAG", String.valueOf(len));
                String ciphertext = encrypt(txt, len);
                Log.d("MY_TAG", ciphertext);
                if (check(ciphertext, getString(R.string.secret))) {
                    Toast.makeText(getApplicationContext(), getString(R.string.success), Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(getApplicationContext(), getString(R.string.failed), Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String sayHello();

    public native String sayHi();

    public native String encrypt(String p, int l);
}