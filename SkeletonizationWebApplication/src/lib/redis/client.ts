import { createClient } from "redis";

const redisUrl = process.env.REDIS_URL ?? "redis://localhost:6379";
const isTls = redisUrl.startsWith("rediss://");

const createRedisClient = () =>
  createClient({
    url: redisUrl,
    socket: {
      tls: isTls,
      keepAlive: 5000,
      connectTimeout: 10000,
      noDelay: true,
      reconnectStrategy: (retries) => {
        const delay = retries < 3 ? 100 : Math.min(retries * 500, 10000);
        console.log(`Redis: Reconnecting in ${delay}ms (attempt ${retries})`);
        return delay;
      }
    }
  });

export type RedisClient = ReturnType<typeof createRedisClient>;

const startHeartbeat = (client: RedisClient, label: string): ReturnType<typeof setInterval> =>
  setInterval(async () => {
    try {
      if (client.isOpen) {
        await client.ping();
      }
    } catch {
      console.warn(`${label}: Heartbeat ping failed`);
    }
  }, 15000);

class RedisClientManager {
  private client: RedisClient | null = null;
  private connectionPromise: Promise<void> | null = null;
  private isConnected = false;
  private heartbeatInterval: ReturnType<typeof setInterval> | null = null;

  async getClient(): Promise<RedisClient> {
    if (this.isConnected && this.client?.isOpen) {
      return this.client;
    }

    if (this.connectionPromise) {
      await this.connectionPromise;
      return this.client!;
    }

    this.connectionPromise = this.connect();
    await this.connectionPromise;
    return this.client!;
  }

  private async connect(): Promise<void> {
    if (this.heartbeatInterval) {
      clearInterval(this.heartbeatInterval);
      this.heartbeatInterval = null;
    }

    try {
      this.client = createRedisClient();

      this.client.on("error", (err: Error) => {
        console.error("Redis Client Error:", err.message);
        this.isConnected = false;
      });
      this.client.on("connect", () => console.log("Redis: Connected"));
      this.client.on("reconnecting", () => {
        console.log("Redis: Reconnecting...");
        this.isConnected = false;
      });
      this.client.on("ready", () => {
        console.log("Redis: Ready");
        this.isConnected = true;
      });
      this.client.on("end", () => {
        console.log("Redis: Connection closed");
        this.isConnected = false;
        this.connectionPromise = null;
      });

      await this.client.connect();
      this.isConnected = true;
      this.heartbeatInterval = startHeartbeat(this.client, "Redis");
      console.log("Redis: Successfully connected");
    } catch (error) {
      console.error("Redis: Failed to connect", error);
      this.connectionPromise = null;
      throw error;
    }
  }

  async createSubscriberClient(): Promise<RedisClient> {
    const client = createRedisClient();
    let heartbeat: ReturnType<typeof setInterval> | null = null;

    client.on("error", (err) => console.error("Redis Subscriber Error:", err.message));
    client.on("reconnecting", () => console.log("Redis Subscriber: Reconnecting..."));
    client.on("ready", () => console.log("Redis Subscriber: Ready"));
    client.on("end", () => {
      console.log("Redis Subscriber: Connection closed");
      if (heartbeat) clearInterval(heartbeat);
    });

    await client.connect();
    heartbeat = startHeartbeat(client, "Redis Subscriber");
    console.log("Redis Subscriber: Connected");
    return client;
  }
}

export const redisClientManager = new RedisClientManager();
