public class Singleton<T> where T : class, new()
{
    public static T GetInstance()
    {
        if(null == instance)
        {
            instance = new T();
        }

        return instance;
    }

    private static T instance = null;
}